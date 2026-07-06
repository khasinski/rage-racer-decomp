#include "common.h"
#include "psyq/gpu.h"

void func_80065B24(Rect *arg0, void *arg1);
s32 func_80064BB4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_800648EC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Rect rect;
    s32 width;

    rect.x = arg3;
    rect.y = arg4;
    rect.h = arg6;

    if (arg1 == 1) {
        goto one;
    }
    if (arg1 < 2) {
        if (arg1 == 0) {
            goto zero;
        }
        goto call;
    }
    if (arg1 == 2) {
        goto two;
    }
    goto call;

zero:
    width = arg5;
    if (width < 0) {
        width += 3;
    }
    rect.w = width >> 2;
    goto call;

one:
    width = arg5;
    width += (u32)arg5 >> 31;
    rect.w = width >> 1;
    goto call;

two:
    rect.w = arg5;

call:
    func_80065B24(&rect, arg0);
    return func_80064BB4(arg1, arg2, arg3, arg4) & 0xFFFF;
}
