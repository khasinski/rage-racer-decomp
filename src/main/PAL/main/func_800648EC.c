#include "common.h"
#include "psyq/gpu.h"

void LoadImage(Rect *arg0, void *arg1) asm("func_80065B24");
s32 GetTPage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80064BB4");

s32 Gpu_LoadTexImageAndGetTPage(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) asm("func_800648EC");

s32 Gpu_LoadTexImageAndGetTPage(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
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
    LoadImage(&rect, arg0);
    return GetTPage(arg1, arg2, arg3, arg4) & 0xFFFF;
}
