#include "common.h"

void LibcMemcpy(u8 *dst, u8 *src, s32 count) asm("func_8006CBF4");
void LibcMemcpy(u8 *dst, u8 *src, s32 count) {
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

s32 LibcStrcmp(u8 *arg0, u8 *arg1) asm("func_8006CC28");
s32 LibcStrcmp(u8 *arg0, u8 *arg1) {
    s32 left;

    if ((arg0 == 0) || (arg1 == 0)) {
        if (arg0 == arg1) {
            return 0;
        }
        if (arg0 == 0) {
            return -1;
        }
        return 1;
    }

    while ((left = *arg0) == *arg1++) {
        if (left == 0) {
            return 0;
        }
        arg0++;
    }

    return *arg0 - arg1[-1];
}

s32 LibcStrncmp(u8 *arg0, u8 *arg1, s32 arg2) asm("func_8006CC8C");
s32 LibcStrncmp(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 left;
    s32 right;

    if ((arg0 == 0) || (arg1 == 0)) {
        if (arg0 == arg1) {
            return 0;
        }
        if (arg0 == 0) {
            return -1;
        }
        return 1;
    }

    arg2--;
    if (arg2 < 0) {
        return 0;
    }

loop:
    left = *arg0;
    right = *arg1++;
    if (left != right) {
        goto mismatch;
    }
    arg0++;
    if (left == 0) {
        return 0;
    }
    arg2--;
    if (arg2 >= 0) {
        goto loop;
    }

mismatch:
    if (arg2 < 0) {
        return 0;
    }
    return *arg0 - arg1[-1];
}
