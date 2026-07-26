#include "common.h"
#include "psyq/gte.h"
#include "game/race.h"
#include "game/track.h"
#include "game/render.h"

extern u32 *g_VisibleCellMask asm("D_801E6828");
extern s32 g_CourseModelCount asm("D_801E40E4");

void func_80017794(void *arg0, void *arg1, Matrix *mtx);

void GameDrawShuttleScenery(s32 arg0) asm("func_8003F4BC");

void GameDrawShuttleScenery(s32 arg0) {
    Matrix mtx0;
    Matrix mtx1;
    register GameShuttleScenery *state asm("$18");
    register GameShuttleScenery *base asm("$3");
    register Matrix *mtx1Ptr asm("$17");
    register s32 drawArg asm("$16");
    register s32 drawValue asm("$5");
    register s32 offset asm("$2");
    register s32 bucket asm("$2");
    register s32 bit asm("$5");
    register s32 firstValue asm("$3");
    register s32 value asm("$6");
    register u32 *visibility asm("$3");
    register u32 *wordPtr asm("$4");
    register s32 visible asm("$3");
    s32 frameValue;

    offset = (((arg0 * 3) << 2) + arg0) << 2;
    base = g_ShuttleScenery;
    state = (GameShuttleScenery *)((u8 *)base + offset);
    firstValue = state->z;
    bucket = firstValue + 0x400;
    if (bucket < 0) {
        bucket = firstValue + 0xBFF;
    }
    bucket >>= 11;
    bucket <<= 2;
    value = state->x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)(bucket + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bucket = bit >> 11;
    visible = 1 << bucket;
    visible &= *wordPtr;

    if ((visible != 0) || (g_CourseIndex == 2)) {
        drawArg = 0x3F;
        GameBuildRotMatrixY(&mtx0, state->angleY);
        mtx1Ptr = &mtx1;
        GameBuildRotMatrixZ(mtx1Ptr, state->angleZ);
        MulMatrix2(&mtx0, mtx1Ptr);
        MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
        if ((g_CourseIndex & 3) >= 2) {
            drawArg = 0x3C;
        }
        func_80017794((void *)0x1F80011C, &state->x, mtx1Ptr);
        frameValue = g_CourseModelCount;
        *(s32 *)0x1F800084 = 0;
        drawValue = 1;
        if (drawArg < frameValue) {
            drawValue = drawArg;
        }
        GameSubmitCourseModel((void *)0x1F800000, drawValue);
    }
}
