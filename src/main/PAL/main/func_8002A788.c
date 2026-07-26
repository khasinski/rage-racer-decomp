#include "common.h"

/*
 * Unsigned angular difference between two 12-bit angles, wrapped to [0, 0x800]
 * (takes the shorter way around the 0x1000 circle). Signed companion is
 * GameGetAngleDelta.
 */
s32 GameGetAngleDistance(s32 arg0, s32 arg1) asm("func_8002A788");
s32 GameGetAngleDistance(s32 arg0, s32 arg1) {
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
