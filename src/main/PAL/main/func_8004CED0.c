#include "common.h"
#include "psyq/gpu.h"

extern s32 g_TeamLogoClutRect asm("D_8007BEDC");
extern s32 D_8007F964;
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void func_8004CED0(void) { LoadImage((Rect *)&g_TeamLogoClutRect, &D_8007F964); }
