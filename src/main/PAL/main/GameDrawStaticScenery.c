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

extern Vec4i D_8007E340;
extern s32 D_8007E34C;
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
    register s32 *statePtr asm("$16");
    register s32 bucket asm("$2");
    register s32 value asm("$6");
    register u32 *visibility asm("$3");
    register u32 *wordPtr asm("$4");
    register s32 bit asm("$5");
    register s32 visible asm("$3");
    register s32 drawArg asm("$5");
    register s32 frameValue asm("$2");

    state = D_8007E340;
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
        GameBuildRotMatrixY(&mtx, D_8007E34C);
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

extern s32 D_8007E35C;

void GameDrawHighClassScenery(void) asm("func_8003E0D0");

void GameDrawHighClassScenery(void) {
    Matrix mtx;
    s32 pad[4];
    register s32 *state asm("$16");
    register s32 frameValue asm("$2");
    register s32 drawArg asm("$5");

    (void)pad;
    state = &D_8007E35C;
    GameBuildRotMatrixY(&mtx, state[0]);
    MulMatrix2((Matrix *)0x1F800028, &mtx);

    if (g_IsEnvironmentMode4 != 0) {
        func_80017794((void *)0x1F80011C, state - 3, &mtx);
        frameValue = g_CourseModelCount;
        *(s32 *)0x1F800084 = 0x10000;
        drawArg = 1;
        if (frameValue >= 0x40) {
            drawArg = 0x3F;
        }
        GameSubmitCourseModel((void *)0x1F800000, drawArg);
    } else {
        func_80017794((void *)0x1F80011C, state - 3, &mtx);
        frameValue = g_CourseModelCount;
        *(s32 *)0x1F800084 = 0;
        drawArg = 1;
        if (frameValue >= 0x40) {
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
    register s32 mode asm("$16") = arg0;
    register s32 value asm("$18") = arg1;
    register s32 flag asm("$17");

    asm("" : "=r"(mode) : "0"(mode));
    asm("" : "=r"(value) : "0"(value));

    flag = arg2;

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
        goto call0;
    case 1:
        if (g_GrandPrixClass >= 2) {
            GameDrawSpinningScenery(value, flag);
        }
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
        }
        GameDrawShuttleScenery(0);
        goto call0;
    case 2:
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
            GameUpdateShuttleScenery(1);
        }
        GameDrawShuttleScenery(0);
        GameDrawShuttleScenery(1);
        goto call0;
    case 3:
        GameDrawAnimatedScenery(value, 1);
        goto call1;
    default:
        return;
    }

call0:
    GameDrawStaticScenery(0);
    return;
call1:
    GameDrawStaticScenery(1);
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
        goto call0;
    case 1:
        if (g_GrandPrixClass >= 2) {
            GameDrawSpinningScenery(value, flag);
        }
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
        }
        GameDrawShuttleScenery(0);
        goto call0;
    case 2:
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
            GameUpdateShuttleScenery(1);
        }
        GameDrawShuttleScenery(0);
        GameDrawShuttleScenery(1);
        goto call0;
    case 3:
        GameDrawAnimatedScenery2(value, 1, g_SceneId == 0x11, flag);
        goto call1;
    default:
        return;
    }

call0:
    GameDrawStaticScenery(0);
    return;
call1:
    GameDrawStaticScenery(1);
}

extern u8 *D_801E4448;
extern s32 g_LapCount asm("D_801E4364");
extern volatile s32 g_RaceSeries asm("D_801E408C");
extern u8 D_801E42FC[];
extern s16 D_801E4308;
extern u8 *D_801E43F4;

s32 GameRandom15(void) asm("func_800632B0");

void func_8003E464(void) {
    register u8 *base asm("s0");
    register u8 *out asm("s1");
    register s32 count asm("a0");
    register s32 value asm("v1");
    register s32 index asm("v0");
    register s32 cmp asm("v0");
    register s32 scene0 asm("v1");
    register s32 scene1 asm("v0");
    register u8 *src asm("v1");
    register s32 recordIndex asm("v1");
    register s32 word0 asm("a0");
    register s32 word1 asm("a1");
    register s32 word2 asm("a2");
    register s32 word3 asm("a3");

    base = D_801E4448;
    index = GameRandom15();
    count = g_LapCount;
    value = index % count;
    out = D_801E42FC;
    value++;
    D_801E4308 = value;
    value = (s16)value;

    if (value <= 0) {
        D_801E4308 = (u16)g_LapCount - 1;
    } else {
        __asm__ volatile("" ::: "memory");
        cmp = count < value;
        if (cmp != 0) {
            D_801E4308 = (u16)g_LapCount;
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
    D_801E43F4 = base + index;
}

/*
 * Ticks the course's one scripted airborne prop and its engine sound; the model
 * is submitted elsewhere. Armed by a scene-counter match, then runs 451 frames,
 * integrating position from a keyframed heading and feeding a distance-attenuated
 * volume to GameSetPitchedSoundCue. See docs/names.md 1.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/GameDrawStaticScenery", func_8003E590);
