#include "common.h"

s32 func_8002A7C4(s32 arg0, s32 arg1) {
    s32 lhs = arg0 & 0xFFF;
    s32 rhs = arg1 & 0xFFF;
    s32 sign = lhs < rhs;
    s32 diff;

    if (sign) {
        diff = rhs - lhs;
    } else {
        diff = lhs - rhs;
    }

    if (diff >= 0x801) {
        asm("");
        diff = 0x1000 - diff;
        sign = !sign;
    }

    {
        register s32 ret asm("$2") = diff;

        if (!sign) {
            ret = -ret;
        }

        return ret;
    }
}
