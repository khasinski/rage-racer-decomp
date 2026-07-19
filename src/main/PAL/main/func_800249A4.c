#include "common.h"

extern u8 *D_8019C900;

s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);
s32 func_80016EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80032F34(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_800249A4(s32 arg0, s32 arg1) {
    s32 b = arg1;
    s32 base = (s32)D_8019C900 + 0xCC;
    s32 next;
    s32 i;
    s32 y;
    register s32 c asm("$17");

    next = func_80017138(base, *(s32 *)0x1F800000, 0x4E, b + 0xA, 0x10, 0xC, 0xB4, 0xC4, 0x7F40);
    next = func_80017138(base, next, 0xE4, b + 0xA, 0x10, 0xC, 0xC4, 0xC4, 0x7F40);
    next = func_80017390(base, next, 0x3A);
    c = 0x46;
    i = 0;
    if (i <= arg0) {
        do {
            y = 0x1C + (i << 3);
            next = func_80016EC4(base, next, c + y, b + 4, 4, 0x18, 0xFC, 0x40, 0x7E82);
            i++;
        } while (i <= arg0);
    }
    next = func_80017390(base, next, 0x39);
    next = func_80032F34(base, next, c | 1, b + 2, 0xB2, 0x1C, 0, 0, 0);
    *(s32 *)0x1F800000 = func_80032F34(base, next, c, b, 0xB4, 0x20, 0xFF, 0xFF, 0xFF);
}
