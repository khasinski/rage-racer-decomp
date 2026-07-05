#include "common.h"

s32 func_8006CC8C(u8 *arg0, u8 *arg1, s32 arg2) {
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
