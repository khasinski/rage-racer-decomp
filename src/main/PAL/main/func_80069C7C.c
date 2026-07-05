#include "common.h"

s32 func_80069C7C(s32 value) {
    register s32 ret asm("$2");

    asm volatile(
        "mtc2 $4,$30\n"
        "nop\n"
        "nop\n"
        "mfc2 $2,$31"
        : "=r"(ret)
        : "r"(value));
    return ret;
}

asm(".globl func_80069C94\n"
    "func_80069C94:\n"
    "nop");
