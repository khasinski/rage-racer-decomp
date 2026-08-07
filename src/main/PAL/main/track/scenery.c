#include "common.h"
#include "game/vector.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/race.h"
#include "game/state.h"
#include "game/audio.h"
#include "game/random.h"
#include "game/track.h"
#include "game/menu.h"

extern u32 *g_VisibleCellMask;

static inline void ClearScratchRenderMode3DF68(void) {
    *(s32 *)0x1F800084 = 0;
}

void DrawStaticScenery(s32 arg0) {
    Matrix mtx;
    Vec4 state;
    s32 *statePtr;
    s32 wordIndex;
    s32 bitIndex;
    s32 value;
    u32 *visibility;
    u32 *wordPtr;
    s32 bit;
    s32 visible;
    s16 drawArg;
    s32 frameValue;

    state = g_StaticSceneryPos;
    statePtr = (s32 *)&state;

    if (arg0 != 0) {
        state.z += 0x5000;
    }

    wordIndex = state.z + 0x400;
    if (wordIndex < 0) {
        wordIndex = state.z + 0xBFF;
    }
    wordIndex >>= 11;

    value = state.x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)((wordIndex << 2) + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bitIndex = bit >> 11;
    visible = 1 << bitIndex;
    visible &= *wordPtr;

    if (visible != 0) {
        BuildRotMatrixY(&mtx, g_StaticSceneryYaw);
        MulMatrix2((Matrix *)0x1F800028, &mtx);

        if (g_IsEnvironmentMode4 != 0) {
            SetGteObjectMatrix((void *)0x1F80011C, statePtr, &mtx);
            frameValue = g_CourseModelCount;
            ClearScratchRenderMode3DF68();
            drawArg = 1;
            if (frameValue >= 0x3B) {
                drawArg = 0x3A;
            }
            SubmitCourseModel((void *)0x1F800000, drawArg);
        } else {
            SetGteObjectMatrix((void *)0x1F80011C, statePtr, &mtx);
            frameValue = g_CourseModelCount;
            ClearScratchRenderMode3DF68();
            drawArg = 1;
            if (frameValue >= 0x3A) {
                drawArg = 0x39;
            }
            SubmitCourseModel2((void *)0x1F800000, drawArg);
        }
    }
}

void DrawHighClassScenery(void) {
    Matrix mtx;
    s32 pad[4];
    s32 *state;
    s32 drawArg;

    (void)pad;
    state = &g_HighClassSceneryYaw;
    BuildRotMatrixY(&mtx, state[0]);
    MulMatrix2((Matrix *)0x1F800028, &mtx);

    if (g_IsEnvironmentMode4 != 0) {
        SetGteObjectMatrix((void *)0x1F80011C, state - 3, &mtx);
        *(s32 *)0x1F800084 = 0x10000;
        drawArg = 1;
        if (g_CourseModelCount >= 0x40) {
            drawArg = 0x3F;
        }
        SubmitCourseModel((void *)0x1F800000, drawArg);
    } else {
        SetGteObjectMatrix((void *)0x1F80011C, state - 3, &mtx);
        *(s32 *)0x1F800084 = 0;
        drawArg = 1;
        if (g_CourseModelCount >= 0x40) {
            drawArg = 0x3F;
        }
        SubmitCourseModel2((void *)0x1F800000, drawArg);
    }
}

void DrawCourseScenery(s32 arg0, s32 arg1, s32 arg2) {
    s32 mode = arg0;
    s32 value = arg1;
    s32 flag = arg2;

    DrawAnimatedScenery(value, 0);

    if (g_GrandPrixClass == 5) {
        flag = 0;
    }

    switch (mode) {
    case 0:
        DrawSpinningScenery(value, flag);
        if (g_GrandPrixClass >= 4) {
            DrawHighClassScenery();
        }
        DrawStaticScenery(0);
        break;
    case 1:
        if (g_GrandPrixClass >= 2) {
            DrawSpinningScenery(value, flag);
        }
        if (flag != 0) {
            UpdateShuttleScenery(0);
        }
        DrawShuttleScenery(0);
        DrawStaticScenery(0);
        break;
    case 2:
        if (flag != 0) {
            UpdateShuttleScenery(0);
            UpdateShuttleScenery(1);
        }
        DrawShuttleScenery(0);
        DrawShuttleScenery(1);
        DrawStaticScenery(0);
        break;
    case 3:
        DrawAnimatedScenery(value, 1);
        DrawStaticScenery(1);
        break;
    default:
        break;
    }
}

void DrawCourseScenery2(s32 arg0, s32 arg1) {
    s32 value = arg0;
    s32 flag = arg1;
    s32 mode;

    if (g_GrandPrixClass == 5) {
        flag = 0;
    }

    DrawAnimatedScenery2(value, 0, g_SceneId == 0x11, flag);

    mode = g_CourseIndex & 3;
    switch (mode) {
    case 0:
        DrawSpinningScenery(value, flag);
        if (g_GrandPrixClass >= 4) {
            DrawHighClassScenery();
        }
        DrawStaticScenery(0);
        break;
    case 1:
        if (g_GrandPrixClass >= 2) {
            DrawSpinningScenery(value, flag);
        }
        if (flag != 0) {
            UpdateShuttleScenery(0);
        }
        DrawShuttleScenery(0);
        DrawStaticScenery(0);
        break;
    case 2:
        if (flag != 0) {
            UpdateShuttleScenery(0);
            UpdateShuttleScenery(1);
        }
        DrawShuttleScenery(0);
        DrawShuttleScenery(1);
        DrawStaticScenery(0);
        break;
    case 3:
        DrawAnimatedScenery2(value, 1, g_SceneId == 0x11, flag);
        DrawStaticScenery(1);
        break;
    default:
        break;
    }
}

extern u8 *g_FlybySceneryData;
extern volatile s32 g_RaceSeries;
extern u8 g_FlybyScenery[];

void SeedFlybyScenery(void) {
    u8 *base;
    register u8 *out asm("s1");
    s32 count;
    s16 value;
    s32 index;
    s32 cmp;
    s32 scene0;
    s32 scene1;
    u8 *src;
    s32 recordIndex;

    base = g_FlybySceneryData;
    index = Random15();
    count = g_LapCount;
    value = index % count;
    out = g_FlybyScenery;
    value++;
    g_FlybySceneryLap = value;
    value = (s16)value;

    if (value <= 0) {
        g_FlybySceneryLap = (u16)g_LapCount - 1;
    } else {
        cmp = count < value;
        if (cmp != 0) {
            g_FlybySceneryLap = (u16)g_LapCount;
        }
    }

    scene0 = g_RaceSeries;
    *(s32 *)(out + 4) = 1;
    scene1 = g_RaceSeries;
    *(s32 *)(out + 0) = 0;

    src = (u8 *)((scene0 * 32) + (s32)base);
    *(Vec4 *)(out + 0x10) = *(Vec4 *)(src + 0x10);
    recordIndex = *(s16 *)((scene1 * 4) + (s32)base + 8);
    *(s32 *)(out + 0x30) = 0;
    index = recordIndex * 3;
    index <<= 2;
    index += 0x50;
    g_FlybySceneryKeyframe = base + index;
}

/*
 * Ticks the course's one scripted airborne prop and its engine sound; the model
 * is submitted elsewhere. Armed by a scene-counter match, then runs 451 frames,
 * integrating position from a keyframed heading and feeding a distance-attenuated
 * volume to SetPitchedSoundCue. See docs/names.md 1.
 */
extern s16 g_PlayerLap;
extern s32 g_PlayerCar;

#define KFREC(off) (*(s16 *)(kf + *(s16 *)(state + 0xE) * 12 + (off)))

void UpdateFlybyScenery(void) {
    Matrix mtxY;
    Matrix mtxX;
    s16 dir[4];
    s32 step[4];
    s32 delta[4];
    u8 *base;
    u8 *state;
    u8 *src;
    s32 series;
    s32 index;
    s32 recordIndex;
    u8 *kf;
    Matrix *mx;
    s32 dt;
    s32 cue;
    s32 pitch;
    s32 vol;
    s32 dist;
    s32 dx;
    s32 dy;
    s32 dz;

    base = g_FlybySceneryData;
    state = g_FlybyScenery;

    if (g_PlayerLap == g_FlybySceneryLap) {
        series = g_RaceSeries;
        if (g_PlayerTrackSection == *(s16 *)((series * 4) + (s32)base)) {
            g_FlybySceneryArmed = 1;
            *(s32 *)(state + 0) = 1;
            src = (u8 *)((series * 32) + (s32)base);
            g_FlybySceneryFrame = 0;
            g_FlybySceneryLap = 0;
            g_FlybySceneryKeyIndex = 0;
            g_FlybySceneryPosRec = *(Vec4 *)(src + 0x10);
            g_FlybySceneryRotZ = 0;
            g_FlybySceneryRotY = 0;
            g_FlybySceneryRotX = 0;
            recordIndex = *(s16 *)((g_RaceSeries * 4) + (s32)base + 8);
            index = recordIndex * 3;
            index <<= 2;
            index += 0x50;
            g_FlybySceneryKeyframe = base + index;
        }
    }

    if (*(s32 *)(state + 0) > 0) {
        *(s32 *)(state + 0) = *(s32 *)(state + 0) + 1;
        *(s32 *)(state + 8) = *(s32 *)(state + 8) + 1;
        if (*(s32 *)(state + 0) >= 0x1C3) {
            *(s32 *)(state + 0) = 0;
        }
        if (*(s16 *)(g_FlybySceneryKeyframe + *(s16 *)(state + 0xE) * 12 + 6) ==
            *(s32 *)(state + 8)) {
            *(s16 *)(state + 0xE) = *(s16 *)(state + 0xE) + 1;
            *(s32 *)(state + 8) = 0;
        }
        if (*(s16 *)(g_FlybySceneryKeyframe + *(s16 *)(state + 0xE) * 12 + 6) == -1) {
            *(s16 *)(state + 0xE) = 0;
        }
        kf = g_FlybySceneryKeyframe;
        *(s32 *)(state + 0x20) =
            (KFREC(0xC) * *(s32 *)(state + 8) +
             KFREC(0) * (dt = KFREC(6) - *(s32 *)(state + 8))) / KFREC(6);
        *(s32 *)(state + 0x24) =
            (KFREC(0xE) * *(s32 *)(state + 8) + KFREC(2) * dt) / KFREC(6);
        *(s32 *)(state + 0x28) =
            (KFREC(0x10) * *(s32 *)(state + 8) + KFREC(4) * dt) / KFREC(6);
        dir[0] = 0;
        dir[1] = 0;
        dir[2] = -KFREC(8) * 4;
        BuildRotMatrixY(&mtxY, 0x800 - *(s32 *)(state + 0x24));
        mx = &mtxX;
        BuildRotMatrixX(mx, *(s32 *)(state + 0x20));
        MulMatrix2(&mtxY, mx);
        BuildRotMatrixZ(&mtxY, *(s32 *)(state + 0x28));
        MulMatrix(mx, &mtxY);
        ApplyMatrix(mx, dir, step);
        *(s32 *)(state + 0x10) = step[0] / 4 + *(s32 *)(state + 0x10);
        *(s32 *)(state + 0x14) = step[1] / 4 + *(s32 *)(state + 0x14);
        *(s32 *)(state + 0x18) = step[2] / 4 + *(s32 *)(state + 0x18);
        if (*(s32 *)(state + 4) == 1) {
            delta[0] = dx = g_PlayerCar - *(s32 *)(state + 0x10);
            delta[1] = dy = g_PlayerCarY - *(s32 *)(state + 0x14);
            delta[2] = dz = g_PlayerCarZ - *(s32 *)(state + 0x18);
            dist = SquareRoot12(dx * dx / 8 + dy * dy / 16 + dz * dz / 8) >> 12;
            if (dist < 0) {
                *(s32 *)(state + 4) = 0;
                dist = 0x74;
            }
            vol = 0x74 - dist;
            if (vol >= 0x75) {
                vol = 0x74;
            }
            if (vol < 0) {
                vol = 0;
            }
            pitch = 0x1900;
            *(s32 *)(state + 0x30) = vol;
        } else {
            vol = 0;
            *(s32 *)(state + 0x30) = 0;
            pitch = 0;
        }
    } else {
        vol = 0;
        *(s32 *)(state + 0x30) = 0;
        pitch = 0;
    }

    if (g_RacePhase >= 3) {
        pitch = 0;
        vol = 0;
    }

    switch (g_CourseIndex & 3) {
    default:
        cue = 1;
        pitch = 0;
        vol = 0;
        break;
    case 0:
        cue = 1;
        break;
    case 1:
        cue = 1;
        pitch = 0;
        vol = 0;
        break;
    case 3:
        cue = 2;
        break;
    case 2:
        cue = 2;
        break;
    }
    SetPitchedSoundCue(cue, pitch, vol);
}
