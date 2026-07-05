#include "common.h"

extern u8 *D_8009A588;
extern volatile u8 D_8009DF20[];
extern volatile u8 D_8009DF22[];
extern volatile u8 D_8009E0A0[];

s32 SsUtGetDetVVol(s32 arg0, s16 *arg1, s16 *arg2) asm("func_800783D8");
s32 SsUtSetDetVVol(s32 arg0, s16 arg1, s16 arg2) asm("func_80078430");

s32 SsUtGetDetVVol(s32 arg0, s16 *arg1, s16 *arg2) {
    register s32 offset asm("$3");
    register u8 *base asm("$2");

    if ((u16)arg0 < 0x18U) {
        offset = (arg0 << 16) >> 12;
        base = D_8009A588;
        *arg1 = *(u16 *)(offset + (s32)base);
        base = D_8009A588;
        offset += (s32)base;
        *arg2 = *(u16 *)(offset + 2);
        return 0;
    }

    return -1;
}

s32 SsUtSetDetVVol(s32 arg0, s16 arg1, s16 arg2) {
    register s32 ret asm("$2");
    register s32 index asm("$3");
    register s32 offset asm("$5");
    register s16 valueX asm("$7");
    register u8 flags asm("$4");

    __asm__ volatile("addiu $sp,$sp,-8" ::: "memory");
    valueX = arg1;

    if ((u16)arg0 >= 0x18U) {
        ret = -1;
        goto done;
    }

    index = (s16)arg0;
    offset = index << 4;
    *(volatile s16 *)(D_8009DF22 + offset) = arg2;
    flags = D_8009E0A0[index];
    ret = 0;
    *(volatile s16 *)(D_8009DF20 + offset) = valueX;
    flags |= 3;
    D_8009E0A0[index] = flags;

done:
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return ret;
}
