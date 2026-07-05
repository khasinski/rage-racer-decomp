#include "common.h"

extern u8 *D_801E4B98;

u32 func_800414C4(s32 arg0, s32 arg1, s32 arg2) {
    register s32 x asm("$4") = arg0 << 2;
    register s32 y asm("$5");
    register u8 *base asm("$2");
    register u32 mask asm("$3");

    asm volatile("" : "=r"(x) : "0"(x));
    y = arg1 << 7;
    base = D_801E4B98;
    mask = 1;
    y += (s32)base;
    x += y;
    return (mask << arg2) & *(u32 *)x;
}
