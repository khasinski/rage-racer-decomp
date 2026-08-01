#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/race.h"
#include "game/state.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4i;

extern Vec4i g_StaticSceneryPos asm("D_8007E340");
/* +0x0C of the same 4-word record, i.e. its w component used as a Y angle;
 * g_HighClassSceneryYaw is the same field of the record at 0x8007E350. */
extern s32 g_StaticSceneryYaw asm("D_8007E34C");
extern u32 *g_VisibleCellMask asm("D_801E6828");
extern s32 g_IsEnvironmentMode4 asm("D_801E4030");
extern s32 g_CourseModelCount asm("D_801E40E4");

void func_80017794(void *arg0, void *arg1, Matrix *mtx);

static inline void ClearScratchRenderMode3DF68(void) {
    *(s32 *)0x1F800084 = 0;
}

void GameDrawStaticScenery(s32 arg0) asm("func_8003DF68");

void GameDrawStaticScenery(s32 arg0) {
    Matrix mtx;
    Vec4i state;
    s32 *statePtr;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 bucket asm("$2");
    s32 value;
    u32 *visibility;
    u32 *wordPtr;
    s32 bit;
    s32 visible;
    s32 drawArg;
    s32 frameValue;

    state = g_StaticSceneryPos;
    statePtr = (s32 *)&state;

    if (arg0 != 0) {
        state.z += 0x5000;
    }

    bucket = state.z + 0x400;
    if (bucket < 0) {
        bucket = state.z + 0xBFF;
    }
    bucket >>= 11;
    bucket <<= 2;

    value = state.x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)(bucket + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bucket = bit >> 11;
    visible = 1 << bucket;
    visible &= *wordPtr;

    if (visible != 0) {
        GameBuildRotMatrixY(&mtx, g_StaticSceneryYaw);
        MulMatrix2((Matrix *)0x1F800028, &mtx);

        if (g_IsEnvironmentMode4 != 0) {
            func_80017794((void *)0x1F80011C, statePtr, &mtx);
            frameValue = g_CourseModelCount;
            ClearScratchRenderMode3DF68();
            drawArg = 1;
            if (frameValue >= 0x3B) {
                drawArg = 0x3A;
            }
            GameSubmitCourseModel((void *)0x1F800000, drawArg);
        } else {
            func_80017794((void *)0x1F80011C, statePtr, &mtx);
            frameValue = g_CourseModelCount;
            ClearScratchRenderMode3DF68();
            drawArg = 1;
            if (frameValue >= 0x3A) {
                drawArg = 0x39;
            }
            GameSubmitCourseModel2((void *)0x1F800000, drawArg);
        }
    }
}

extern s32 g_HighClassSceneryYaw asm("D_8007E35C");

void GameDrawHighClassScenery(void) asm("func_8003E0D0");

void GameDrawHighClassScenery(void) {
    Matrix mtx;
    s32 pad[4];
    s32 *state;
    s32 drawArg;

    (void)pad;
    state = &g_HighClassSceneryYaw;
    GameBuildRotMatrixY(&mtx, state[0]);
    MulMatrix2((Matrix *)0x1F800028, &mtx);

    if (g_IsEnvironmentMode4 != 0) {
        func_80017794((void *)0x1F80011C, state - 3, &mtx);
        *(s32 *)0x1F800084 = 0x10000;
        drawArg = 1;
        if (g_CourseModelCount >= 0x40) {
            drawArg = 0x3F;
        }
        GameSubmitCourseModel((void *)0x1F800000, drawArg);
    } else {
        func_80017794((void *)0x1F80011C, state - 3, &mtx);
        *(s32 *)0x1F800084 = 0;
        drawArg = 1;
        if (g_CourseModelCount >= 0x40) {
            drawArg = 0x3F;
        }
        GameSubmitCourseModel2((void *)0x1F800000, drawArg);
    }
}

void GameDrawAnimatedScenery(s32 arg0, s32 arg1) asm("func_8003D6F0");
void GameDrawSpinningScenery(s32 arg0, s32 arg1) asm("func_8003DDAC");
void GameUpdateShuttleScenery(s32 arg0) asm("func_8003F2A4");
void GameDrawShuttleScenery(s32 arg0) asm("func_8003F4BC");

void GameDrawCourseScenery(s32 arg0, s32 arg1, s32 arg2) asm("func_8003E1A4");

void GameDrawCourseScenery(s32 arg0, s32 arg1, s32 arg2) {
    s32 mode = arg0;
    s32 value = arg1;
    s32 flag = arg2;

    GameDrawAnimatedScenery(value, 0);

    if (g_GrandPrixClass == 5) {
        flag = 0;
    }

    switch (mode) {
    case 0:
        GameDrawSpinningScenery(value, flag);
        if (g_GrandPrixClass >= 4) {
            GameDrawHighClassScenery();
        }
        GameDrawStaticScenery(0);
        break;
    case 1:
        if (g_GrandPrixClass >= 2) {
            GameDrawSpinningScenery(value, flag);
        }
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
        }
        GameDrawShuttleScenery(0);
        GameDrawStaticScenery(0);
        break;
    case 2:
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
            GameUpdateShuttleScenery(1);
        }
        GameDrawShuttleScenery(0);
        GameDrawShuttleScenery(1);
        GameDrawStaticScenery(0);
        break;
    case 3:
        GameDrawAnimatedScenery(value, 1);
        GameDrawStaticScenery(1);
        break;
    default:
        break;
    }
}

void GameDrawAnimatedScenery2(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_8003DA90");

void GameDrawCourseScenery2(s32 arg0, s32 arg1) asm("func_8003E2E8");

void GameDrawCourseScenery2(s32 arg0, s32 arg1) {
    s32 value = arg0;
    s32 flag = arg1;
    s32 mode;

    if (g_GrandPrixClass == 5) {
        flag = 0;
    }

    GameDrawAnimatedScenery2(value, 0, g_SceneId == 0x11, flag);

    mode = g_CourseIndex & 3;
    switch (mode) {
    case 0:
        GameDrawSpinningScenery(value, flag);
        if (g_GrandPrixClass >= 4) {
            GameDrawHighClassScenery();
        }
        GameDrawStaticScenery(0);
        break;
    case 1:
        if (g_GrandPrixClass >= 2) {
            GameDrawSpinningScenery(value, flag);
        }
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
        }
        GameDrawShuttleScenery(0);
        GameDrawStaticScenery(0);
        break;
    case 2:
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
            GameUpdateShuttleScenery(1);
        }
        GameDrawShuttleScenery(0);
        GameDrawShuttleScenery(1);
        GameDrawStaticScenery(0);
        break;
    case 3:
        GameDrawAnimatedScenery2(value, 1, g_SceneId == 0x11, flag);
        GameDrawStaticScenery(1);
        break;
    default:
        break;
    }
}

extern u8 *g_FlybySceneryData asm("D_801E4448");
extern s32 g_LapCount asm("D_801E4364");
extern volatile s32 g_RaceSeries asm("D_801E408C");
extern u8 g_FlybyScenery[] asm("D_801E42FC");
extern s16 g_FlybySceneryLap asm("D_801E4308");
extern u8 *g_FlybySceneryKeyframe asm("D_801E43F4");

s32 GameRandom15(void) asm("func_800632B0");

void GameSeedFlybyScenery(void) asm("func_8003E464");
void GameSeedFlybyScenery(void) {
    u8 *base;
    /* These pins are load-bearing: removing any one changes .text. */
    register u8 *out asm("s1");
    s32 count;
    register s32 value asm("v1");
    s32 index;
    register s32 cmp asm("v0");
    s32 scene0;
    register s32 scene1 asm("v0");
    u8 *src;
    s32 recordIndex;
    s32 word0;
    s32 word1;
    s32 word2;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 word3 asm("a3");

    base = g_FlybySceneryData;
    index = GameRandom15();
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

    src = (u8 *)((scene0 << 5) + (s32)base);
    word0 = *(s32 *)(src + 0x10);
    word1 = *(s32 *)(src + 0x14);
    word2 = *(s32 *)(src + 0x18);
    word3 = *(s32 *)(src + 0x1C);
    *(s32 *)(out + 0x10) = word0;
    *(s32 *)(out + 0x14) = word1;
    *(s32 *)(out + 0x18) = word2;
    *(s32 *)(out + 0x1C) = word3;

    __asm__ volatile("" ::: "memory");
    recordIndex = *(s16 *)((scene1 << 2) + (s32)base + 8);
    *(s32 *)(out + 0x30) = 0;
    index = (recordIndex << 1) + recordIndex;
    index <<= 2;
    index += 0x50;
    g_FlybySceneryKeyframe = base + index;
}

/*
 * Ticks the course's one scripted airborne prop and its engine sound; the model
 * is submitted elsewhere. Armed by a scene-counter match, then runs 451 frames,
 * integrating position from a keyframed heading and feeding a distance-attenuated
 * volume to GameSetPitchedSoundCue. See docs/names.md 1.
 */
extern s16 g_PlayerLap asm("D_8009E83C");
extern s16 g_PlayerTrackSection asm("D_8009E74C");
extern s32 g_FlybySceneryArmed asm("D_801E4300");
extern s32 g_FlybySceneryFrame asm("D_801E4304");
extern s16 g_FlybySceneryKeyIndex asm("D_801E430A");
extern Vec4i g_FlybySceneryPosRec asm("D_801E430C");
extern s32 g_FlybySceneryRotX2 asm("D_801E431C");
extern s32 g_FlybySceneryRotY2 asm("D_801E4320");
extern s32 g_FlybySceneryRotZ2 asm("D_801E4324");
extern s32 g_RaceSeriesNV asm("D_801E408C");
extern s32 g_PlayerCarX asm("D_8009E6D4");
extern s32 g_PlayerCarY asm("D_8009E6D8");
extern s32 g_PlayerCarZ asm("D_8009E6DC");

void GameSetPitchedSoundCue(s32 cue, s32 pitch, s32 volume) asm("func_8005C914");

#define KFREC(off) (*(s16 *)(kf + *(s16 *)(state + 0xE) * 12 + (off)))

void GameUpdateFlybyScenery(void) asm("func_8003E590");

void GameUpdateFlybyScenery(void) {
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
        if (g_PlayerTrackSection == *(s16 *)((series << 2) + (s32)base)) {
            g_FlybySceneryArmed = 1;
            *(s32 *)(state + 0) = 1;
            src = (u8 *)((series << 5) + (s32)base);
            g_FlybySceneryFrame = 0;
            g_FlybySceneryLap = 0;
            g_FlybySceneryKeyIndex = 0;
            g_FlybySceneryPosRec = *(Vec4i *)(src + 0x10);
            g_FlybySceneryRotZ2 = 0;
            g_FlybySceneryRotY2 = 0;
            g_FlybySceneryRotX2 = 0;
            recordIndex = *(s16 *)((g_RaceSeriesNV << 2) + (s32)base + 8);
            index = (recordIndex << 1) + recordIndex;
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
        GameBuildRotMatrixY(&mtxY, 0x800 - *(s32 *)(state + 0x24));
        mx = &mtxX;
        GameBuildRotMatrixX(mx, *(s32 *)(state + 0x20));
        MulMatrix2(&mtxY, mx);
        GameBuildRotMatrixZ(&mtxY, *(s32 *)(state + 0x28));
        MulMatrix(mx, &mtxY);
        ApplyMatrix(mx, dir, step);
        *(s32 *)(state + 0x10) = step[0] / 4 + *(s32 *)(state + 0x10);
        *(s32 *)(state + 0x14) = step[1] / 4 + *(s32 *)(state + 0x14);
        *(s32 *)(state + 0x18) = step[2] / 4 + *(s32 *)(state + 0x18);
        if (*(s32 *)(state + 4) == 1) {
            delta[0] = dx = g_PlayerCarX - *(s32 *)(state + 0x10);
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
    GameSetPitchedSoundCue(cue, pitch, vol);
}
