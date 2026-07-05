#include "common.h"

void MDEC_in(volatile u32 *arg0, s32 arg1) asm("func_800641D0");

void DecDCTin(volatile u32 *arg0, s32 arg1) asm("func_80063FB0");
void DecDCTin(volatile u32 *arg0, s32 arg1) {
    u32 mask;

    if (arg1 & 1) {
        arg0[0] &= 0xF7FFFFFF;
    } else {
        arg0[0] |= 0x08000000;
    }

    mask = 0x02000000;
    if (arg1 & 2) {
        arg0[0] |= mask;
    } else {
        arg0[0] &= 0xFDFFFFFF;
    }

    MDEC_in(arg0, *(u16 *)arg0);
}
