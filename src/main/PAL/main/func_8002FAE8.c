#include "common.h"

s32 func_8002FAE8(s32 arg0, s32 arg1, s32 arg2) {
    s32 lhs = arg0 & 0xFFF;
    s32 rhs = arg1 & 0xFFF;
    s32 inv = 0x400 - arg2;
    s32 sum;

    if (rhs < lhs) {
        if (lhs - rhs >= 0x801) {
            rhs += 0x1000;
        }
    } else if (rhs - lhs >= 0x801) {
        lhs += 0x1000;
    }

    sum = lhs * inv + rhs * arg2;
    if (sum < 0) {
        sum += 0x3FF;
    }

    return (sum >> 10) & 0xFFF;
}
