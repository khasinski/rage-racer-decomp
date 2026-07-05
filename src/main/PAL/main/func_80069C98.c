#include "common.h"

s32 func_80069C98(s32 arg0, s32 arg1, s32 arg2) {
    register s32 ret asm("$2");

    asm volatile(
        "mtc2 $4,$12\n"
        "mtc2 $6,$14\n"
        "mtc2 $5,$13\n"
        "nop\n"
        "nop\n"
        "cop2 0x1400006\n"
        "mfc2 $2,$24"
        : "=r"(ret)
        : "r"(arg0), "r"(arg1), "r"(arg2));
    return ret;
}
