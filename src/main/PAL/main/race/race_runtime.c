#include "common.h"
#include "game/track_internal.h"
#include "game/prim.h"
#include "game/audio.h"
#include "game/car.h"
#include "game/player_car_internal.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/vector.h"
#include "game/waypoint.h"
#include "psyq/gpu.h"
#include "psyq/gte.h"


/*
 * Waypoint proximity test: returns 1 if the waypoint's (x,y) lies within a
 * +/-0x40 box around the car centre (g_PlayerCar / g_PlayerCarZ), else 0.
 */

/*
 * Per-frame waypoint spawn/update state machine over the 6 slots. An idle slot
 * (active==0) that the car is near (IsCarNearWaypoint) spawns: increments the spawn
 * counter g_WaypointsCollected, plays cue 0xA, marks the slot active and seeds its
 * velocity from g_PlayerVelocity. An active slot integrates position from velocity
 * with 15/16 per-frame damping and grows its scale toward 0x400, retiring to
 * state 2 once motion decays to zero. Register pins and raw tail-relative field
 * offsets are match-load-bearing.
 */

/* Counts how many of the 6 waypoint slots are active (active != 0). */
















/*
 * The tail's multiply feeds the discarded rounding path below. GCC 2.6.3
 * removes that path and its mflo, but leaves the mult that sets the hard HI/LO
 * registers behind (docs/names.md 31f and 35c).
 */





/*
 * Initializes/spawns a route render object `ent`: reads a start entry from the
 * per-scene table (`arr` indexed by `pos`, g_TrackEventData base), sets the model id
 * (+0xAE / +0x122), start angle (0xC00 - track angle), zeroes the motion state
 * block, resolves the containing track point (FindTrackSegment) and builds the
 * initial marker geometry (UpdateCarTrackState). `ent` is a render/route object
 * accessed by raw byte offset (its first 0xE8 mirror GameRenderObject).
 */

s32 IsCarNearWaypoint(TrackWaypointRuntime *waypoint) {
    s32 center_x = g_PlayerCar.x;
    s32 x = waypoint->x;
    s32 ret = 0;

    if ((center_x - 0x40) < x) {
        s32 max_x = center_x + 0x40;

        if (x < max_x) {
            s32 center_y = g_PlayerCarZ;
            s32 y = waypoint->y;

            if ((center_y - 0x40) < y) {
                s32 max_y = center_y + 0x40;

                ret = y < max_y;
            }
        }
    }

    return ret;
}

void UpdateWaypoints(void) {
    TrackWaypointRuntime *waypoint;
    s32 i;
    s32 activeState;
    /*
     * Retail addresses the waypoint's velocity block through a base register
     * biased to &waypoint->velocityMagnitude, so these stay raw:
     *   tail-0x30 x        tail-0x28 y          tail-0x20 scale
     *   tail-0x1C field18  tail-0x10 velocityX  tail-0xC field28
     *   tail-0x8 velocityY tail-0x4  field30    tail      velocityMagnitude
     * Writing them as waypoint->field drops the second induction variable and
     * re-bases every store on $17.
     */
    register char *tail asm("$16");

    if (g_WaypointSpawnCooldown != 0) {
        g_WaypointSpawnCooldown--;
    }

    waypoint = g_Waypoints;
    i = 0;
    activeState = 1;
    tail = (char *)waypoint + 0x34;
    do {
        if (waypoint->active == 0) {
            if (IsCarNearWaypoint(waypoint) != 0) {
                g_WaypointsCollected++;
                PlaySoundCue(0xA);

                waypoint->active = activeState;
                *(Block16 *)(tail - 0x10) = *(Block16 *)g_PlayerVelocity;

                *(s32 *)(tail - 0x10) *= 2;
                *(s32 *)(tail - 0x8) *= 2;
                *(s32 *)tail =
                    ((*(s32 *)(tail - 0x10) * *(s32 *)(tail - 0x10)) + (*(s32 *)(tail - 0x8) * *(s32 *)(tail - 0x8))) /
                    0x2000;
            }
        } else if (waypoint->active == activeState) {
            *(s32 *)(tail - 0x30) += *(s32 *)(tail - 0x10) / 0x100;
            *(s32 *)(tail - 0x28) += *(s32 *)(tail - 0x8) / 0x100;
            *(s32 *)(tail - 0x10) = (*(s32 *)(tail - 0x10) * 15) / 16;
            *(s32 *)(tail - 0x8) = (*(s32 *)(tail - 0x8) * 15) / 16;
            *(s32 *)(tail - 0x1C) += *(s32 *)tail / 0x100;
            *(s32 *)tail = (*(s32 *)tail * 15) / 16;

            if (*(s32 *)(tail - 0x20) < 0x400) {
                *(s32 *)(tail - 0x20) += 0x80;
            } else {
                *(s32 *)(tail - 0x20) = 0x400;
            }

            if ((*(s32 *)(tail - 0x10) == 0) && (*(s32 *)(tail - 0x8) == 0) && (*(s32 *)tail == 0)) {
                waypoint->active = 2;
            }
        }

        i++;
        tail += sizeof(*waypoint);
        waypoint++;
    } while (i < 6);
}

static inline void ClearScratchRenderMode37AAC(void) {
    g_ScratchRenderMode = 0;
}

/*
 * Renders the 6 waypoints. For each active-shaped slot it builds a rotation
 * matrix from the waypoint's angle (point+0x14) and tilt (point+0x10) and emits
 * two GTE draw primitives (SubmitModel) into the scratchpad OT: the second is
 * the same billboard rotated by 0x800 (180 degrees). `point` walks the
 * TrackWaypointRuntime array g_Waypoints via raw offsets. Register pins are
 * match-load-bearing.
 */
void DrawWaypoints(void) {
    Matrix mtx0;
    Matrix mtx1;
    s32 drawId;
    s32 i;
    Matrix *mtx1Ptr;
    register char *point asm("$16");
    s32 frameValue;
    s32 drawArg;

    drawId = 2;
    SelectModelBank(0);
    i = 0;
    mtx1Ptr = &mtx1;
    point = (char *)&g_Waypoints[0].x;

    do {
        BuildRotMatrixY(&mtx0, *(s32 *)(point + 0x14));
        MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &mtx0);
        BuildRotMatrixZ(mtx1Ptr, *(s32 *)(point + 0x10));
        MulMatrix(&mtx0, mtx1Ptr);
        SetGteObjectMatrix((void *)0x1F80011C, point, &mtx0);
        frameValue = g_ModelBankCount;
        ClearScratchRenderMode37AAC();
        drawArg = 1;
        if (drawId < frameValue) {
            drawArg = drawId;
        }
        SubmitModel((void *)SCRATCHPAD_ADDR, drawArg);

        BuildRotMatrixY(mtx1Ptr, 0x800);
        MulMatrix2(&mtx0, mtx1Ptr);
        SetGteObjectMatrix((void *)0x1F80011C, point, mtx1Ptr);
        frameValue = g_ModelBankCount;
        ClearScratchRenderMode37AAC();
        drawArg = 1;
        if (drawId < frameValue) {
            drawArg = drawId;
        }
        SubmitModel((void *)SCRATCHPAD_ADDR, drawArg);

        i++;
        point += sizeof(TrackWaypointRuntime);
    } while (i < 6);
}

s32 CountActiveWaypoints(void) {
    TrackWaypointRuntime *ptr = g_Waypoints;
    s32 count = 0;
    s32 i = 5;

    do {
        i--;
        count += ptr->active != 0;
        ptr++;
    } while (i >= 0);

    return count;
}

void DrawLapNumber(void) {
    SPRT *scratch;
    s32 track;
    s32 divisor;
    s32 digitsDrawn;
    s32 xOffset;
    s32 quotient;
    register SPRT *packet asm("$16");

    scratch = SCRATCH_PRIM_CURSOR_AS(SPRT);
    track = g_PlayerLap;
    divisor = 1;
    digitsDrawn = 0;
    xOffset = 0;
    packet = scratch;

    while (1) {
        quotient = track / divisor;
        if (quotient == 0 && digitsDrawn > 0) {
            break;
        }

        {
            s32 y;
            SPRT *oldPacket;
            s32 tens;

            SetSprt(scratch);
            SetShadeTex(scratch, 1);

            y = 0x120 - xOffset;
            oldPacket = packet;
            tens = quotient / 10;
            divisor *= 10;
            xOffset += 0x18;
            digitsDrawn++;
            scratch++;
            packet->v0 = 0x48;
            packet->w = 0x18;
            packet->h = 0x20;
            packet->y0 = 0x10;
            packet->clut = 0x780B;
            packet->x0 = y;
            packet->u0 = (quotient - tens * 10) * 24;

            packet++;
            AddPrim(g_DrawBuffer + 0xCC, oldPacket);
        }
    }

    {
        void *ot;
        u8 *finalScratch;
        s32 tpage;

        finalScratch = (u8 *)scratch;
        packet = (SPRT *)SCRATCHPAD_ADDR;
        ot = g_DrawBuffer + 0xCC;
        tpage = 9;
        *(u8 **)packet = finalScratch;
        *(u8 **)packet = QueueDrawModePrim(ot, finalScratch, tpage);
    }
}

void DrawEndingScreen(void) {
    s16 *p;
    u32 sceneTimer;
    s32 x = 0;

    g_SceneTimer = g_SceneTimer + 1;
    if ((u32)g_SceneTimer < 61) {
        DrawFullscreenFadeTile(255 - (g_SceneTimer - 6) * 11, 0x49);
    }
    if ((u32)g_SceneTimer >= 571 && g_EndingSceneLatch == 0) {
        g_EndingSceneLatch = 1;
    }

    if (g_PlayerProgressB + g_PlayerProgressA >= g_PlayerLap * g_TrackLength) {
        if (g_PlayerLap < 257) {
            g_PlayerLap = g_PlayerLap + 1;
            SeedWaypoints();
        }
    }
    if (g_PlayerLap >= 257) {
        if (g_RacePhase == 2) {
            g_RacePhase = 4;
            g_RaceFadeTimer = 0;
        }
    }

    if (g_RacePhase == 5) {
        if (g_RaceFadeTimer > 0) {
            DrawRaceEndBanner(g_RaceFadeTimer * 3);
            DrawFullscreenFadeTile(g_RaceFadeTimer * 3, 0x49);
            x = 6;
        }
        if (g_RaceFadeTimer >= 101) {
            ExitRaceScene(x);
        }
        g_RaceFadeTimer = g_RaceFadeTimer + 1;
    } else if (g_RacePhase == 4) {
        DrawText8x8(0x5c, 0x78, &g_TextCongratulations, 0x7811);
        DrawFullscreenFadeTile(g_RaceFadeTimer * 2, 0x29);
        g_RaceFadeTimer = g_RaceFadeTimer + 1;
        if (g_RaceFadeTimer < 201) {
            g_RaceFadeTimer = g_RaceFadeTimer + 1;
        } else {
            ExitRaceScene(6);
        }
    }

    switch (0) { default:
    sceneTimer = g_SceneTimer;
    g_AnimTimer = g_AnimTimer + 1;
    if (sceneTimer >= 90) {
        if (g_RacePhase == 0) {
            g_RacePhase = 1;
            break;
        }
    } else {
        if (g_RacePhase == 0) {
            RunRaceIntroCamera((struct Obj *)&g_PlayerCar, sceneTimer);
            g_EndingSceneLatch = 0;
            g_WaypointsCollected = 0;
            break;
        }
    }
    if (g_RacePhase == 1) {
        if ((u32)g_SceneTimer >= 211) {
            BeginCarStandingStart(&g_PlayerCar, sceneTimer);
            g_RacePhase = 2;
        }
    }
    }

    if (g_RacePhase < 4) {
        DrawStartCountdown(g_SceneTimer);
        PlayCountdownCues(g_SceneTimer);
    }

    if (g_RacePhase > 0) {
        UpdatePlayerCar(&g_PlayerCar);
    } else if (g_RacePhase == 0) {
        UpdateLoadedAudioVoices(0, 1);
    }
    DrawLapNumber();

    if (g_RacePhase > 0) {
        UpdateCamera(0, &g_PlayerCar);
    }

    p = &g_PlayerTrackSection;
    RequestTrackTexturePage(*p);
    UpdateEnvironment();
    DrawSkyBackground();
    SCRATCH_ENV_MODE4 = g_IsEnvironmentMode4;
    DrawTerrainCells();
    DrawCourseObjects();
    DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 1);
    GetTrackZoneBlend(g_PlayerTrackProgress);
    SetReverbDepth(g_ReverbZoneDepth, g_ReverbZoneDepth);
    DrawPlayerTachometer();
    UpdateTrackEventSound(*p);
    if (g_RacePhase < 3) {
        UpdateWaypoints();
        DrawWaypoints();
    }
}

void ApplyTrackReverbZone(s32 position) {
    s32 result;
    s32 i;
    register s32 offset;
    s32 depth;
    register s32 scene;

    result = 0;
    if (position < 0) {
        position += g_TrackLength;
    }

    scene = g_RaceSeries;
    offset = scene * 16;
    for (i = 0; i < 2; i++) {
        if (*(s32 *)((u8 *)&g_ReverbZoneStart + offset) < position) {
            if (position < *(s32 *)((u8 *)&g_ReverbZoneEnd + offset)) {
                result = 0x46;
                break;
            }
        }
        offset += 8;
    }

    depth = result;
    SetReverbDepth(depth, depth);
}

s32 GetWaypointAngle(s32 position) {
    s32 trackLength;
    s32 value;
    register s32 temp asm("v0");
    s32 angle;
    register s32 remainder asm("a0");
    register s32 scale;

    trackLength = g_TrackLength;
    temp = 0xB875;
    value = trackLength + temp;
    value -= position;
    remainder = value % trackLength;
    value = remainder;

    if (value < 0x1A97) {
        temp = value / 100;
        value = 0x80 - temp;
    } else {
        value = trackLength - remainder;
        if (value < 0x801) {
            temp = value / 10;
            value = 0x80 - temp;
        } else {
            value = 0;
        }
    }

    if (value != 0) {
        temp = Atan2(0x29DD - SCRATCH_VIEW_X, 0x6EF3 - SCRATCH_VIEW_Z);
        value = 0xC00 - temp;
        temp = SCRATCH_VIEW_ANGLE_Y;
        value &= 0xFFF;
        temp -= value;
        angle = temp & 0xFFF;
        temp = rsin(angle);
        temp = rsin(angle);
        scale = -0x40;
        value = temp * scale;
        if (value < 0) {
            value += 0xFFF;
        }
        asm(".globl func_80038368\nfunc_80038368 = GetWaypointAngle + 0xE0");
        return temp;
    }
}

void InitRivalCar(GameCarRuntime *ent, s32 pos, s32 *arr) {
    u8 *base;
    s32 sub;
    u8 *p;
    u16 val122;
    s32 scene;
    u16 av;

    ent->field_B0 = 1;
    av = *(u16 *)&arr[pos];
    sub = (pos + 1) * 12;
    {
        u8 *baseValue = g_TrackEventData;
        base = baseValue;
    }
    ent->field_8A = 0;
    ent->field_BC = 1;
    ent->field_AE = av;
    val122 = *(u16 *)&arr[pos];
    scene = g_RaceSeries;
    ent->field_122 = val122;
    {
        RivalStartEntry *p1;

        p1 = (RivalStartEntry *)(base + (sub + scene * 144) + 0x354);
        ent->trackPointIndex = p1->trackPointIndex;
        ent->x = p1->x;
        ent->z = p1->z;
        ent->y = 0;
    }
    {
        s32 ret = FindTrackSegment((u8 *)ent, ent->trackPointIndex);
        s32 lev = g_RaceSeries;
        s32 idx;
        s32 levShift;
        s32 acc;
        s32 angle;

        ent->trackPointIndex = ret;
        ent->field_20 = 0;
        idx = ent->trackPointIndex;
        acc = 0xC00;
        levShift = lev << 11;
        angle = *(s16 *)((u8 *)g_TrackPoints + idx * 24 + 0xA);
        acc -= levShift;
        ent->field_24 = (acc - angle) & 0xFFF;

        ent->field_28 = 0;
        ent->field_64 = 0;
        ent->field_6C = 0;
        ent->field_68 = 0;
        ent->trackProgress = 0;
        ent->field_A4 = 0;
        ent->field_A8 = 0;
        ent->field_D0 = 0;
        ent->field_CC = 0;
        ent->field_C8 = 0;
        ent->field_E0 = 0;
        ent->field_DC = 0;
        ent->field_D8 = 0;
        ent->motionZ = 0;
        ent->motionY = 0;
        ent->motionX = 0;
        ent->routeIndex = 0;
        ent->field_116 = 0;
        ent->field_110 = 0;
        ent->field_F4 = 0;
        ent->routeMarkerActive = 0;
        ent->slideInput.value = 0;
        ent->field_108 = ent->field_24;
        p = base + (sub + lev * 144);
        ent->field_EC = ent->field_24;
        ent->headingAngle = ent->field_24;
        ent->field_F8 = 0;
        ent->field_104 = 0;
        ent->field_C4 = 0;
        ent->routeMarkerIndex = 0;
        SeedCarLapProgress(ent, ((RivalStartEntry *)(p + 0x354))->modelId);
    }

    sub += g_RaceSeries * 144;
    base += sub;
    {
        u16 model;

        model = ((RivalStartEntry *)(base + 0x354))->modelId;
        ent->activeFlag = model;
        if ((s16)model != -1) {
            struct {
                s32 pad[4];
                s16 a;
                s16 b;
            } pair;

            pair.a = 20;
            pair.b = -20;
            UpdateCarTrackState((u8 *)ent, ent->trackPointIndex, &pair.a);
            ent->field_60 = ent->y;
            ent->previousTrackProgress = ent->trackProgress;
        }
    }

    {
        s32 height;

        height = ent->field_34;
        ent->field_120 = 0;
        ent->field_FC = height;
        ent->field_11E = height;
        ent->field_11C = height;
    }
    *(Vec4 *)&ent->field_50 = *(Vec4 *)&ent->field_20;
    {
        s32 lateral;

        lateral = ent->y;
        ent->field_40 = 0;
        ent->field_44 = 0;
        ent->field_48 = 0;
        ent->field_60 = lateral;
    }
}

void InitRivalCarAi(GameCarRuntime *ent, s32 pos, s32 *arr) {
  s32 pos2_R10;
  s32 idx_R8;
  register u8 *base_R9;
  register GameCarRuntime *ent2_R7;
  GameCarAiBlock *sub_R6;
  s32 c;
  u16 w;
  pos2_R10 = pos;
  idx_R8 = arr[pos2_R10];
  base_R9 = g_TrackEventData;
  ent2_R7 = ent;
  if (!(idx_R8 < 12))
  {
    idx_R8 = 0;
  }
  {
    s32 lev1_R3;
    unsigned int idxoff1_R4;
    register RivalAiConfig *p1_R4;
    lev1_R3 = g_RaceSeries;
    idxoff1_R4 = idx_R8;
    idxoff1_R4 = idxoff1_R4 * 16;
    p1_R4 = (RivalAiConfig *)(base_R9 + (idxoff1_R4 + (lev1_R3 * 192)) + 0x8F4);
    ent2_R7->field_124 = (p1_R4->speed * 1168) / 160;
    ent2_R7->field_126 = p1_R4->field_126;
    ent2_R7->field_128 = p1_R4->field_128;
    ent2_R7->field_12A = p1_R4->field_12A;
    ent2_R7->field_12C = p1_R4->field_12C;
  }
  __asm__ volatile("");
  c = ent2_R7->field_128;
  sub_R6 = (GameCarAiBlock *)&ent2_R7->field_BC;
  ent2_R7->field_12E = 0;
  if (c < 0)
  {
    ent2_R7->field_128 = 0;
  }
  else
    if (!(c < 11))
  {
    ent2_R7->field_128 = 10;
  }
  if ((sub_R6->field_12A) < 0)
  {
    sub_R6->field_12A = 0;
  }
  c = sub_R6->field_12C;
  if (c <= 0)
  {
    sub_R6->field_12C = 0;
  }
  else
    if (!(c < 16))
  {
    sub_R6->field_12C = 15;
  }
  {
    s32 lev2_R2;
    s32 idxoff2_R4;
    register u8 *p2_R3 asm("$3");
    lev2_R2 = g_RaceSeries;
    idxoff2_R4 = idx_R8 * 16;
    p2_R3 = base_R9 + (idxoff2_R4 + (lev2_R2 * 192));
    w = ((RivalAiConfig *)(p2_R3 + 0x8F4))->field_132;
    sub_R6->field_132 = w;
    if (((s16) w) < 0x3D)
    {
      sub_R6->field_132 = 0x3C;
    }
    lev2_R2 = g_RaceSeries;
    __asm__("" : "=r"(idxoff2_R4) : "0"(idxoff2_R4));
    p2_R3 = base_R9 + (idxoff2_R4 + (lev2_R2 * 192));
    w = ((RivalAiConfig *)(p2_R3 + 0x8F4))->field_134;
    sub_R6->field_134 = w;
    if (((s16) w) <= 0)
    {
      sub_R6->field_134 = 0;
    }
  }
  {
    s32 v_R3;
    v_R3 = sub_R6->field_124;
    sub_R6->field_130 = (v_R3 * 6) / 100;
  }
  if (pos2_R10 >= 4)
  {
    s32 d_R5;
    d_R5 = g_TrackLength;
    sub_R6->field_118 = (d_R5 / 12) + ((d_R5 / 40) * (pos2_R10 - 4));
  }
  else
  {
    sub_R6->field_118 = g_TrackLength / 12;
  }
}
