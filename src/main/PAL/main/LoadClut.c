#include "common.h"
#include "psyq/gpu.h"

void LoadImage(Rect *rect, void *data) asm("func_80065B24");
s32 func_80064C7C(s32 arg0, s32 arg1);

s32 LoadClut(void *arg0, s32 arg1, s32 arg2) asm("func_80064A40");
s32 LoadClut(void *arg0, s32 arg1, s32 arg2) {
    Rect rect;

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x10;
    rect.h = 1;

    LoadImage(&rect, arg0);
    return func_80064C7C(arg1, arg2) & 0xFFFF;
}
