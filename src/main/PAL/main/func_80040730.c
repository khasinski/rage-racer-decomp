#include "common.h"
#include "psyq/gte.h"
#include "game/state.h"
#include "game/render.h"

extern s16 D_801E4DCA;
extern s16 D_801E4DC8;
extern s16 D_801E4DCC;
extern s16 D_801E4DB8;
extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 D_1F800084;

void func_80017794(void *arg0, void *arg1, Matrix *mtx);

void func_80040730(void) {
    Matrix mtx0;
    Matrix mtx1;
    volatile s32 pad[4];
    s32 drawId;
    s32 frameValue;
    register s16 *anglePtr asm("$16");
    register Matrix *mtx1Ptr asm("$17");
    register Matrix *mtx0Ptr asm("$4");
    register void *scratchVec asm("$4");

    mtx0Ptr = &mtx0;
    __asm__("" : "=r"(mtx0Ptr) : "0"(mtx0Ptr));
    anglePtr = &D_801E4DCA;
    mtx1Ptr = &mtx1;

    GameBuildRotMatrixY(mtx0Ptr, 0x800 - anglePtr[0]);
    GameBuildRotMatrixX(mtx1Ptr, D_801E4DC8);
    MulMatrix2(&mtx0, mtx1Ptr);
    MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
    GameBuildRotMatrixZ(&mtx0, D_801E4DCC);
    MulMatrix2(mtx1Ptr, &mtx0);

    GameSelectModelBank(1);
    scratchVec = (void *)0x1F80011C;
    __asm__("" : "=r"(scratchVec) : "0"(scratchVec));
    anglePtr = (s16 *)((u8 *)anglePtr - 0x12);
    func_80017794(scratchVec, anglePtr, &mtx0);
    frameValue = g_ModelBankCount;
    *(s32 *)0x1F800084 = 0;
    drawId = 1;
    if (frameValue >= 0x24) {
        drawId = 0x23;
    }
    GameSubmitModel((void *)0x1F800000, drawId);

    {
        register s32 base asm("$3");
        register s32 acc asm("$5");
        register s32 tmp asm("$2");

        base = g_SceneTimer;
        acc = base << 2;
        acc += base;
        tmp = acc << 5;
        acc += tmp;
        acc <<= 1;
        acc += base;
        GameBuildRotMatrixY(mtx1Ptr, acc & 0xFFF);
    }
    MulMatrix2(&mtx0, mtx1Ptr);
    func_80017794((void *)0x1F80011C, anglePtr, mtx1Ptr);
    frameValue = g_ModelBankCount;
    D_1F800084 = 0;
    drawId = 1;
    if (frameValue >= 0x25) {
        drawId = 0x24;
    }
    GameSubmitModel((void *)0x1F800000, drawId);
}
