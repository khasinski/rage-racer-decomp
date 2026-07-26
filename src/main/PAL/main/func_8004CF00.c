#include "common.h"
#include "psyq/gpu.h"

extern s32 D_8007BEDC;
extern s32 D_801E444C;
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void func_8004CF00(void) { LoadImage((Rect *)&D_8007BEDC, &D_801E444C); }
