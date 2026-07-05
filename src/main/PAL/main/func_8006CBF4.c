#include "common.h"

void func_8006CBF4(u8 *dst, u8 *src, s32 count) {
    u8 scratch[8];
    register s32 i asm("$3");
    register s32 end asm("$6");

    /* Forces the target's otherwise unused 8-byte leaf frame. */
    asm volatile("" : "=m"(scratch));

    if (count == 0) {
        return;
    }

    i = count - 1;
    end = -1;
loop:
    *dst = *src;
    src++;
    i--;
    dst++;
    if (i != end) {
        goto loop;
    }
}
