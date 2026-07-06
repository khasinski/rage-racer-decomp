#include "common.h"
#include "psyq/gpu.h"

void func_80065B24(Rect *arg0, void *arg1);
s32 func_80064C7C(s32 arg0, s32 arg1);

s32 func_800649D8(void *arg0, s32 arg1, s32 arg2) {
    Rect rect;

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x100;
    rect.h = 1;

    func_80065B24(&rect, arg0);
    return func_80064C7C(arg1, arg2) & 0xFFFF;
}
