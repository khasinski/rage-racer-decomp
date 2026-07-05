#include "common.h"


s32 func_800690E0(s32 *arg0, s32 *arg1, s32 arg2) {
    register s32 xy asm("$8");
    register s32 z asm("$9");

    xy = arg0[0];
    z = arg0[1];
    asm volatile(
        "mtc2 $8,$0\n"
        "mtc2 $9,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "swc2 $9,0($5)\n"
        "swc2 $10,4($5)\n"
        "swc2 $11,8($5)"
        :
        : "r"(xy), "r"(z), "r"(arg1)
        : "memory");
    asm volatile("move $2,$6");
}
