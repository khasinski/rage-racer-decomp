#include "common.h"
extern s32 D_8007BEDC;
extern s32 D_8007F964;
void LoadImage(void *arg0, void *arg1) asm("func_80065B24");
void func_8004CED0(void) { LoadImage(&D_8007BEDC, &D_8007F964); }
