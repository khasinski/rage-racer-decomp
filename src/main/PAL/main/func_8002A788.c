#include "common.h"

s32 func_8002A788(s32 arg0, s32 arg1) {
    s32 lhs = arg0 & 0xFFF;
    s32 rhs = arg1 & 0xFFF;
    s32 diff;

    if (lhs < rhs) {
        diff = rhs - lhs;
    } else {
        diff = lhs - rhs;
    }

    if (diff >= 0x801) {
        diff = 0x1000 - diff;
    }

    return diff;
}
