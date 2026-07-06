#include "common.h"
#include "psyq/gpu.h"

extern s32 D_8007BEDC;
extern s32 D_801E444C;
void func_80065B24(Rect *arg0, void *arg1);

void func_8004CF00(void) { func_80065B24((Rect *)&D_8007BEDC, &D_801E444C); }
