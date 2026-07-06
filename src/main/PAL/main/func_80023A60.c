#include "common.h"

extern u8 *D_8019C900;

s32 func_8001720C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);

void func_80023A60(s32 color, s32 arg1) {
    u8 *base;
    s32 *scratch;
    s32 next;
    s32 width;
    s32 height;
    s32 scratchValue;

    base = D_8019C900 + 0xCC;
    if (color < 0) {
        color = 0;
    } else if (color >= 0x100) {
        color = 0xFF;
    }

    width = 0x140;
    height = 0x1E0;
    scratch = (s32 *)0x1F800000;
    scratchValue = *scratch;
    next = func_8001720C(base, scratchValue, 0, 0, width, height, color, color, color);
    *scratch = func_80017390(base, next, arg1);
}
