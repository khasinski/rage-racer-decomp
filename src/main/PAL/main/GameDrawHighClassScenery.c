#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

extern s32 D_8007E35C;
extern s32 g_IsEnvironmentMode4 asm("D_801E4030");
extern s32 g_CourseModelCount asm("D_801E40E4");

void func_80017794(void *arg0, void *arg1, Matrix *mtx);

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
