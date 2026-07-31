#include "common.h"
#include "game/car.h"
#include "game/audio.h"
#include "game/waypoint.h"
#include "psyq/gte.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"

extern s32 g_PlayerCar asm("D_8009E6D4");

extern s32 g_PlayerCarZ asm("D_8009E6DC");

/*
 * Waypoint proximity test: returns 1 if the waypoint's (x,y) lies within a
 * +/-0x40 box around the car centre (g_PlayerCar / g_PlayerCarZ), else 0.
 */

extern s32 g_WaypointSpawnCooldown asm("D_8019C700");

extern TrackWaypointRuntime g_Waypoints[] asm("D_801E4DF4");

extern s32 g_WaypointsCollected asm("D_801E43F8");

/* g_PlayerCar + 0xC4: the four words a spawning waypoint copies as its own
 * starting velocity. */
extern s32 g_PlayerVelocity[] asm("D_8009E798");

s32 GameIsCarNearWaypoint(TrackWaypointRuntime *waypoint) asm("func_80037808");

/*
 * Per-frame waypoint spawn/update state machine over the 6 slots. An idle slot
 * (active==0) that the car is near (GameIsCarNearWaypoint) spawns: increments the spawn
 * counter g_WaypointsCollected, plays cue 0xA, marks the slot active and seeds its
 * velocity from g_PlayerVelocity. An active slot integrates position from velocity
 * with 15/16 per-frame damping and grows its scale toward 0x400, retiring to
 * state 2 once motion decays to zero. Register pins and raw tail-relative field
 * offsets are match-load-bearing.
 */

extern s32 g_ModelBankCount asm("D_801E4168");


void func_80017794(void *arg0, void *arg1, Matrix *mtx);






extern u32 g_ScratchRenderMode asm("D_1F800084");

/* Counts how many of the 6 waypoint slots are active (active != 0). */

extern s16 g_PlayerLap asm("D_8009E83C");


void *func_80017390(void *ot, void *packet, s32 arg2);

void AddPrim(void *ot, void *prim) asm("func_80064DDC");

void SetShadeTex(u8 *prim, s32 enabled) asm("func_80064EB8");

void SetSprt(u8 *prim) asm("func_80064FA8");


/* Deliberately raw: set to 1 once g_SceneTimer passes 571 and cleared during
 * race phase 0, but its only reader is the guard on its own write, so it has
 * no effect at all -- and this whole scene is unreachable (names.md 15f). */
extern s16 D_8009EC88;


extern s32 g_PlayerProgressA asm("D_8009E73C");

extern s32 g_PlayerProgressB asm("D_8009E740");






extern s32 g_PlayerTrackProgress asm("D_8009E744");

extern s16 g_ReverbZoneDepth asm("D_8019C78C");

extern s16 g_PlayerTrackSection asm("D_8009E74C");

extern u8 g_TextCongratulations asm("D_80011494");

void GameDrawFullscreenFadeTile(s32 a, s32 b) asm("func_80033AA0");

void GameSeedWaypoints(void) asm("func_80037714");

void func_800218A0(s32 a);

void GameExitRaceScene(s32 a) asm("func_80035258");

void func_80016754(s32 a, s32 b, void *c, s32 d);

void GameBeginCarStandingStart(void *a) asm("func_8002BE18");

void GameRunRaceIntroCamera(void *a) asm("func_8003C508");

void GameUpdatePlayerCar(void *a) asm("func_8002DEFC");

void func_8005D9F8(s32 a, s32 b);

void GameDrawLapNumber(void) asm("func_80037C04");

void GameUpdateCamera(s32 a, void *b) asm("func_80043BCC");

void GameRequestTrackTexturePage(s32 a) asm("func_80019EFC");


void func_800418D4(void);

void GameDrawTerrainCells(void) asm("func_80041840");


void GameDrawCourseScenery(s32 a, s32 b, s32 c) asm("func_8003E1A4");

void GameGetTrackZoneBlend(s32 a) asm("func_800350B4");

void func_8005B190(s32 a, s32 b);

void GameDrawPlayerTachometer(void) asm("func_8002F458");

void func_8004087C(s32 a);

void func_8003425C(u32 a);

void GamePlayCountdownCues(u32 a) asm("func_800410BC");

void GameUpdateWaypoints(void) asm("func_80037860");

void GameDrawWaypoints(void) asm("func_80037AAC");


/* Two 8-byte { start, end } reverb spans per series (offset = series * 16):
 * inside one of them GameApplyTrackReverbZone drives the reverb depth to 0x46,
 * outside it to 0. */
extern s32 g_ReverbZoneStart asm("D_8007E054");

extern s32 g_ReverbZoneEnd asm("D_8007E058");

/*
 * The tail's multiply feeds the discarded rounding path below. GCC 2.6.3
 * removes that path and its mflo, but leaves the mult that sets the hard HI/LO
 * registers behind (docs/names.md 31f and 35c).
 */



s32 func_80068568(s32 arg0);


extern u8 *g_TrackPoints asm("D_8009E688");
extern s32 g_TrackLength asm("D_801E40D8");
extern u8 *g_TrackEventData asm("D_801E4150");

extern s32 GameFindTrackSegment(u8 *ent, s32 arg) asm("func_80030EB4");

extern void GameSeedCarLapProgress(u8 *ent, s32 arg) asm("func_8002BF68");

extern void GameUpdateCarTrackState(u8 *ent, s32 arg, void *arg2) asm("func_80031298");

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4;

/*
 * Initializes/spawns a route render object `ent`: reads a start entry from the
 * per-scene table (`arr` indexed by `pos`, g_TrackEventData base), sets the model id
 * (+0xAE / +0x122), start angle (0xC00 - track angle), zeroes the motion state
 * block, resolves the containing track point (GameFindTrackSegment) and builds the
 * initial marker geometry (GameUpdateCarTrackState). `ent` is a render/route object
 * accessed by raw byte offset (its first 0xE8 mirror GameRenderObject).
 */

s32 GameIsCarNearWaypoint(TrackWaypointRuntime *arg0) asm("func_80037808");
s32 GameIsCarNearWaypoint(TrackWaypointRuntime *arg0) {
    s32 center_x = g_PlayerCar;
    s32 x = arg0->x;
    s32 ret = 0;

    if ((center_x - 0x40) < x) {
        s32 max_x = center_x + 0x40;

        if (x < max_x) {
            s32 center_y = g_PlayerCarZ;
            s32 y = arg0->y;

            if ((center_y - 0x40) < y) {
                s32 max_y = center_y + 0x40;

                ret = y < max_y;
            }
        }
    }

    return ret;
}

void GameUpdateWaypoints(void) asm("func_80037860");
void GameUpdateWaypoints(void) {
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
    /* This pin is load-bearing: removing it changes .text. */
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
            if (GameIsCarNearWaypoint(waypoint) != 0) {
                /* These pins are load-bearing: removing any one changes .text. */
                register volatile s32 *src asm("$5");
                register s32 src0 asm("$2");
                s32 src1;
                s32 src2;

                g_WaypointsCollected++;
                GamePlaySoundCue(0xA);

                waypoint->active = activeState;
                asm volatile("" ::: "memory");
                src = g_PlayerVelocity;
                asm volatile("" : "=r"(src) : "0"(src));
                src0 = src[0];
                src1 = src[1];
                src2 = src[2];
                *(s32 *)(tail - 0x10) = src0;
                *(s32 *)(tail - 0xC) = src1;
                *(s32 *)(tail - 0x8) = src2;
                src0 = src[3];
                *(s32 *)(tail - 0x4) = src0;
                asm volatile("" ::: "memory");

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
 * two GTE draw primitives (GameSubmitModel) into the scratchpad OT: the second is
 * the same billboard rotated by 0x800 (180 degrees). `point` walks the
 * TrackWaypointRuntime array g_Waypoints via raw offsets. Register pins are
 * match-load-bearing.
 */
void GameDrawWaypoints(void) asm("func_80037AAC");
void GameDrawWaypoints(void) {
    Matrix mtx0;
    Matrix mtx1;
    s32 drawId;
    s32 i;
    Matrix *mtx1Ptr;
    /* This pin is load-bearing: removing it changes .text. */
    register char *point asm("$16");
    s32 frameValue;
    s32 drawArg;

    drawId = 2;
    GameSelectModelBank(0);
    i = 0;
    mtx1Ptr = &mtx1;
    point = (char *)&g_Waypoints[0].x;

    do {
        GameBuildRotMatrixY(&mtx0, *(s32 *)(point + 0x14));
        MulMatrix2((Matrix *)0x1F800028, &mtx0);
        GameBuildRotMatrixZ(mtx1Ptr, *(s32 *)(point + 0x10));
        MulMatrix(&mtx0, mtx1Ptr);
        func_80017794((void *)0x1F80011C, point, &mtx0);
        frameValue = g_ModelBankCount;
        ClearScratchRenderMode37AAC();
        drawArg = 1;
        if (drawId < frameValue) {
            drawArg = drawId;
        }
        GameSubmitModel((void *)0x1F800000, drawArg);

        GameBuildRotMatrixY(mtx1Ptr, 0x800);
        MulMatrix2(&mtx0, mtx1Ptr);
        func_80017794((void *)0x1F80011C, point, mtx1Ptr);
        frameValue = g_ModelBankCount;
        ClearScratchRenderMode37AAC();
        drawArg = 1;
        if (drawId < frameValue) {
            drawArg = drawId;
        }
        GameSubmitModel((void *)0x1F800000, drawArg);

        i++;
        point += sizeof(TrackWaypointRuntime);
    } while (i < 6);
}

s32 GameCountActiveWaypoints(void) asm("func_80037BD4");
s32 GameCountActiveWaypoints(void) {
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

void GameDrawLapNumber(void) asm("func_80037C04");
void GameDrawLapNumber(void) {
    u8 *scratch;
    s32 track;
    s32 divisor;
    s32 digitsDrawn;
    s32 xOffset;
    s32 quotient;
    /* This pin is load-bearing: removing it changes .text. */
    register u8 *packet asm("$16");

    scratch = *(u8 **)0x1F800000;
    track = g_PlayerLap;
    divisor = 1;
    digitsDrawn = 0;
    xOffset = 0;
    packet = scratch;

    while (1) {
        quotient = track / divisor;
        if (quotient != 0) {
            goto drawDigit;
        }
        if (digitsDrawn > 0) {
            goto done;
        }

drawDigit:
        {
            s32 y;
            u8 *oldPacket;
            s32 tens;

            SetSprt(scratch);
            SetShadeTex(scratch, 1);

            y = 0x120 - xOffset;
            oldPacket = packet;
            tens = quotient / 10;
            divisor *= 10;
            xOffset += 0x18;
            digitsDrawn++;
            scratch += 0x14;
            packet[0x0D] = 0x48;
            *(s16 *)(packet + 0x10) = 0x18;
            *(s16 *)(packet + 0x12) = 0x20;
            *(s16 *)(packet + 0x0A) = 0x10;
            *(s16 *)(packet + 0x0E) = 0x780B;
            *(s16 *)(packet + 0x08) = y;
            packet[0x0C] = (quotient - tens * 10) * 24;

            packet += 0x14;
            AddPrim(g_DrawBuffer + 0xCC, oldPacket);
        }
    }

done:
    {
        void *ot;
        u8 *finalScratch;
        s32 arg2;

        finalScratch = scratch;
        packet = (u8 *)0x1F800000;
        ot = g_DrawBuffer + 0xCC;
        arg2 = 9;
        *(u8 **)packet = finalScratch;
        *(u8 **)packet = func_80017390(ot, finalScratch, arg2);
    }
}

void GameUpdateWaypointCollectScene(void) asm("func_80037D90");
void GameUpdateWaypointCollectScene(void) {
    s16 *p;
    /* This pin is load-bearing: removing it changes .text. */
    register u32 a asm("$5");
    s32 x = 0;

    g_SceneTimer = g_SceneTimer + 1;
    if ((u32)g_SceneTimer < 61) {
        GameDrawFullscreenFadeTile(255 - (g_SceneTimer - 6) * 11, 0x49);
    }
    if ((u32)g_SceneTimer >= 571 && D_8009EC88 == 0) {
        D_8009EC88 = 1;
    }

    if (g_PlayerProgressB + g_PlayerProgressA >= g_PlayerLap * g_TrackLength) {
        if (g_PlayerLap < 257) {
            g_PlayerLap = g_PlayerLap + 1;
            GameSeedWaypoints();
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
            func_800218A0(g_RaceFadeTimer * 3);
            GameDrawFullscreenFadeTile(g_RaceFadeTimer * 3, 0x49);
            x = 6;
        }
        if (g_RaceFadeTimer >= 101) {
            GameExitRaceScene(x);
        }
        g_RaceFadeTimer = g_RaceFadeTimer + 1;
    } else if (g_RacePhase == 4) {
        func_80016754(0x5c, 0x78, &g_TextCongratulations, 0x7811);
        GameDrawFullscreenFadeTile(g_RaceFadeTimer * 2, 0x29);
        g_RaceFadeTimer = g_RaceFadeTimer + 1;
        if (g_RaceFadeTimer < 201) {
            g_RaceFadeTimer = g_RaceFadeTimer + 1;
        } else {
            GameExitRaceScene(6);
        }
    }

    a = g_SceneTimer;
    g_AnimTimer = g_AnimTimer + 1;
    asm volatile("");
    if (a >= 90) {
        asm volatile("" :: "r"(a));
        if (g_RacePhase == 0) {
            g_RacePhase = 1;
            goto Lend;
        }
    } else {
        if (g_RacePhase == 0) {
            GameRunRaceIntroCamera(&g_PlayerCar);
            D_8009EC88 = 0;
            g_WaypointsCollected = 0;
            goto Lend;
        }
    }
    if (g_RacePhase == 1) {
        if ((u32)g_SceneTimer >= 211) {
            GameBeginCarStandingStart(&g_PlayerCar);
            g_RacePhase = 2;
        }
    }
Lend:

    if (g_RacePhase < 4) {
        func_8003425C(g_SceneTimer);
        GamePlayCountdownCues(g_SceneTimer);
    }

    if (g_RacePhase > 0) {
        GameUpdatePlayerCar(&g_PlayerCar);
    } else if (g_RacePhase == 0) {
        func_8005D9F8(0, 1);
    }
    GameDrawLapNumber();

    if (g_RacePhase > 0) {
        GameUpdateCamera(0, &g_PlayerCar);
    }

    p = &g_PlayerTrackSection;
    GameRequestTrackTexturePage(*p);
    GameUpdateEnvironment();
    func_800418D4();
    *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
    GameDrawTerrainCells();
    GameDrawCourseObjects();
    GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 1);
    GameGetTrackZoneBlend(g_PlayerTrackProgress);
    func_8005B190(g_ReverbZoneDepth, g_ReverbZoneDepth);
    GameDrawPlayerTachometer();
    func_8004087C(*p);
    if (g_RacePhase < 3) {
        GameUpdateWaypoints();
        GameDrawWaypoints();
    }
}

void GameApplyTrackReverbZone(s32 arg0) asm("func_800381EC");
void GameApplyTrackReverbZone(s32 arg0) {
    s32 result;
    s32 i;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 offset asm("$3");
    s32 arg;
    register s32 scene asm("$2");

    result = 0;
    if (arg0 < 0) {
        arg0 += g_TrackLength;
    }

    scene = g_RaceSeries;
    i = 0;
    offset = scene << 4;
loop:
    if (*(s32 *)((u8 *)&g_ReverbZoneStart + offset) >= arg0) {
        goto next;
    }

    if (arg0 >= *(s32 *)((u8 *)&g_ReverbZoneEnd + offset)) {
        i++;
        goto check;
    }

    result = 0x46;
    goto done;

next:
    i++;

check:
    if (i < 2) {
        offset += 8;
        goto loop;
    }

done:
    arg = result;
    func_8005B190(arg, arg);
}

s32 func_80038288(s32 arg0) {
    s32 trackLength;
    s32 value;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 temp asm("v0");
    s32 angle;
    register s32 remainder asm("a0");
    register s32 scale;

    trackLength = g_TrackLength;
    temp = 0xB875;
    value = trackLength + temp;
    value -= arg0;
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
        temp = GameAtan2(0x29DD - *(s32 *)0x1F800008, 0x6EF3 - *(s32 *)0x1F800010);
        value = 0xC00 - temp;
        temp = *(s32 *)0x1F80001C;
        value &= 0xFFF;
        temp -= value;
        angle = temp & 0xFFF;
        temp = func_80068568(angle);
        temp = func_80068568(angle);
        scale = -0x40;
        value = temp * scale;
        if (value < 0) {
            value += 0xFFF;
        }
        asm(".globl func_80038368\nfunc_80038368 = func_80038288 + 0xE0");
        return temp;
    }
}

void GameInitRivalCar(GameCarRuntime *ent, s32 pos, s32 *arr) asm("func_800383A8");
void GameInitRivalCar(GameCarRuntime *ent, s32 pos, s32 *arr) {
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
        u8 *p1;

        p1 = base + (sub + scene * 144);
        ent->trackPointIndex = *(s16 *)(p1 + 0x35C);
        ent->x = *(s32 *)(p1 + 0x354);
        ent->z = *(s32 *)(p1 + 0x358);
        *(s32 *)&ent->y = 0;
    }
    {
        s32 ret = GameFindTrackSegment((u8 *)ent, ent->trackPointIndex);
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
        angle = *(s16 *)(g_TrackPoints + idx * 24 + 0xA);
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
        ent->field_18 = 0;
        ent->field_14 = 0;
        ent->field_10 = 0;
        ent->routeIndex = 0;
        ent->field_116 = 0;
        ent->field_110 = 0;
        ent->field_F4 = 0;
        ent->field_13A = 0;
        *(s32 *)&ent->field_F0 = 0;
        ent->field_108 = ent->field_24;
        p = base + (sub + lev * 144);
        ent->field_EC = ent->field_24;
        ent->headingAngle = ent->field_24;
        ent->field_F8 = 0;
        ent->field_104 = 0;
        ent->field_C4 = 0;
        ent->field_138 = 0;
        GameSeedCarLapProgress((u8 *)ent, *(s16 *)(p + 0x35E));
    }

    sub += g_RaceSeries * 144;
    base += sub;
    {
        u16 model;

        model = *(u16 *)(base + 0x35E);
        ent->activeFlag = model;
        if ((s16)model != -1) {
            struct {
                s32 pad[4];
                s16 a;
                s16 b;
            } pair;

            pair.a = 20;
            pair.b = -20;
            GameUpdateCarTrackState((u8 *)ent, ent->trackPointIndex, &pair.a);
            ent->field_60 = *(s32 *)&ent->y;
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

        lateral = *(s32 *)&ent->y;
        ent->field_40 = 0;
        ent->field_44 = 0;
        ent->field_48 = 0;
        ent->field_60 = lateral;
    }
}

void GameInitRivalCarAi(GameCarRuntime *ent, s32 pos, s32 *arr) asm("func_800385FC");
void GameInitRivalCarAi(GameCarRuntime *ent, s32 pos, s32 *arr) {
  s32 pos2_R10;
  s32 idx_R8;
  /* These pins are load-bearing: removing any one changes .text. */
  register u8 *base_R9 asm("$9");
  register GameCarRuntime *ent2_R7 asm("$7");
  GameCarAiBlock *sub_R6;
  s32 c;
  u16 w;
  pos2_R10 = pos;
  __asm__("" : "=r"(pos2_R10) : "0"(pos2_R10));
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
    /* This pin is load-bearing: removing it changes .text. */
    register u8 *p1_R4 asm("$4");
    lev1_R3 = g_RaceSeries;
    idxoff1_R4 = idx_R8;
    idxoff1_R4 = idxoff1_R4 * 16;
    p1_R4 = base_R9 + (idxoff1_R4 + (lev1_R3 * 192));
    ent2_R7->field_124 = ((*((s16 *) (p1_R4 + 0x8F4))) * 1168) / 160;
    *(u16 *)&ent2_R7->field_126 = *((u16 *) (p1_R4 + 0x8F6));
    *(u16 *)&ent2_R7->field_128 = *((u16 *) (p1_R4 + 0x8F8));
    *(u16 *)&ent2_R7->field_12A = *((u16 *) (p1_R4 + 0x8FA));
    *(u16 *)&ent2_R7->field_12C = *((u16 *) (p1_R4 + 0x8FC));
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
    /* This pin is load-bearing: removing it changes .text. */
    register u8 *p2_R3 asm("$3");
    lev2_R2 = g_RaceSeries;
    idxoff2_R4 = idx_R8 * 16;
    p2_R3 = base_R9 + (idxoff2_R4 + (lev2_R2 * 192));
    w = *((u16 *) (p2_R3 + 0x8FE));
    sub_R6->field_132 = w;
    if (((s16) w) < 0x3D)
    {
      sub_R6->field_132 = 0x3C;
    }
    lev2_R2 = g_RaceSeries;
    __asm__("" : "=r"(idxoff2_R4) : "0"(idxoff2_R4));
    p2_R3 = base_R9 + (idxoff2_R4 + (lev2_R2 * 192));
    w = *((u16 *) (p2_R3 + 0x900));
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
    /* This pin is load-bearing: removing it changes .text. */
    register s32 pm4_R3 asm("$3");
    d_R5 = g_TrackLength;
    pm4_R3 = pos2_R10 - 4;
    sub_R6->field_118 = (d_R5 / 12) + ((d_R5 / 40) * pm4_R3);
  }
  else
  {
    sub_R6->field_118 = g_TrackLength / 12;
  }
}
