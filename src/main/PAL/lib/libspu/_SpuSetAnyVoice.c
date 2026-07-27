#include "common.h"

extern volatile u16 *D_8009AB7C;

u32 _SpuSetAnyVoice(s32 arg0, u32 arg1, s32 arg2, s32 arg3) asm("func_8007A21C");

u32 _SpuSetAnyVoice(s32 arg0, u32 arg1, s32 arg2, s32 arg3) {
    register volatile u16 *reg_hi asm("$7");
    register volatile u16 *reg_lo asm("$6");
    register u32 hi asm("$2");
    register u32 lo asm("$3");
    u32 old;

    {
        register volatile u16 *base asm("$3") = D_8009AB7C;

        reg_hi = (volatile u16 *)((u32)(arg3 << 1) + (u32)base);
        reg_lo = (volatile u16 *)((u32)(arg2 << 1) + (u32)base);
    }

    hi = *reg_hi;
    lo = *reg_lo;
    hi &= 0xFF;
    hi <<= 0x10;
    old = lo | hi;

    if (arg0 != 0) {
        if (arg0 != 1) {
            return old & 0xFFFFFF;
        }

        old |= arg1 & 0xFFFFFF;
        *reg_lo |= arg1;
        *reg_hi |= (arg1 >> 0x10) & 0xFF;
    } else {
        old &= ~(arg1 & 0xFFFFFF);
        *reg_lo &= ~arg1;
        *reg_hi &= ~((arg1 >> 0x10) & 0xFF);
    }

    return old & 0xFFFFFF;
}
