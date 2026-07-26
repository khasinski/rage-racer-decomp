#include "common.h"
extern s32 D_80094300;
extern s32 D_80094304;
s32 VSync(s32 mode) asm("func_8006DD30");
void func_80067F04(void) { D_80094300 = VSync(-1) + 240; D_80094304 = 0; }
