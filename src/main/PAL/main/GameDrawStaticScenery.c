#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

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
