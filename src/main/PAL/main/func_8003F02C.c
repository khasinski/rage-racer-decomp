#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 D_801E4340;
extern s32 D_801E4350;
extern volatile s32 D_801E4354;
extern s32 D_801E4358;

void func_80017794(void *arg0, void *arg1, Matrix *mtx);


void func_8003F02C(void) {
    Matrix mtx0;
    Matrix mtx1;
    register Matrix *mtx1Ptr asm("$16");
    s32 frameValue;
    s32 drawId;

    GameBuildRotMatrixY(&mtx0, 0x800 - D_801E4354);
    mtx1Ptr = &mtx1;
    GameBuildRotMatrixX(mtx1Ptr, D_801E4350);
    MulMatrix2(&mtx0, mtx1Ptr);
    MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
    GameBuildRotMatrixZ(&mtx0, D_801E4358);
    MulMatrix2(mtx1Ptr, &mtx0);
    GameSelectModelBank(1);
    func_80017794((void *)0x1F80011C, &D_801E4340, &mtx0);
    frameValue = g_ModelBankCount;
    *(s32 *)0x1F800084 = 0;
    drawId = 1;
    if (frameValue >= 0x26) {
        drawId = 0x25;
    }
    GameSubmitModel((void *)0x1F800000, drawId);
}
