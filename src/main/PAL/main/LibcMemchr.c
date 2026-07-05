#include "common.h"

u8 *LibcMemchr(u8 *arg0, s32 arg1, s32 arg2) asm("func_80063B4C");
u8 *LibcMemchr(u8 *arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0) {
        return 0;
    }
    if (arg2 <= 0) {
        return 0;
    }

    goto start;

found:
    return arg0 - 1;

start:

    arg2--;
    if (arg2 < 0) {
        return 0;
    }

    arg1 &= 0xFF;
    do {
        if (*arg0++ == arg1) {
            goto found;
        }
        arg2--;
    } while (arg2 >= 0);

    return 0;
}
