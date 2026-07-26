#include "common.h"

extern volatile u8 *D_800993D0;
extern volatile u32 *D_800993E8;
extern u8 *D_800993EC;
extern char D_80013B14[];


/*
 * Low-level DMA-channel transfer helper. Spins on the channel's CHCR busy bit
 * (register window at 0x1F801088 + ch*0x10, timing out after 0x10000 polls),
 * then programs MADR/BCR/CHCR to start a transfer of `count` blocks of `size`
 * words. `mode` selects between the block-mode / linked-list setups.
 */
void func_8006DB74(s32 ch, u32 madr, u32 count, u32 size, u32 chcrVal, u8 mode) {
    volatile s32 dummy;
    s32 i;
    volatile u32 *p;
    register u8 *dptr asm("$3");
    register volatile u32 *dp asm("$4");

    i = 0;
    while (*(volatile u32 *)(0x1F801088 + (ch << 4)) & 0x01000000) {
        if (i == 0x10000) {
            GameDebugPrintf(D_80013B14, *(volatile u32 *)(0x1F801088 + (ch << 4)));
            break;
        }
        i++;
    }

    if (mode == 1) {
        register s32 bv asm("$4");
        dptr = D_800993EC;
        bv = dptr[2];
        dptr[2] = bv | (1 << ch);
    } else {
        register s32 bv asm("$4");
        dptr = D_800993EC;
        bv = dptr[2];
        dptr[2] = bv & ~(1 << ch);
    }

    dummy = *(volatile u32 *)D_800993EC;
    asm volatile("");
    {
        register s32 dv asm("$6");
        s32 bit;

        dv = ch * 4;
        bit = 1 << (dv + 3);
        asm volatile("");
        p = (volatile u32 *)(0x1F801080 + (ch << 4));
        dp = D_800993E8;
        dv = *dp;
        *dp = dv | bit;
        *p++ = madr;
        *p++ = (count << 16) | size;
        while (!(*D_800993D0 & 0x40)) {
        }
        *p = chcrVal;
        dummy = *p;
    }
}
