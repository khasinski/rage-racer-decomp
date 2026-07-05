#include "common.h"
extern s32 D_8007BEDC;
extern s32 D_801E444C;
void LoadImage(void *arg0, void *arg1) asm("func_80065B24");
void func_8004CF00(void) { LoadImage(&D_8007BEDC, &D_801E444C); }
