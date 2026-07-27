#include <sys/types.h>

#include "common.h"

extern volatile u_char *D_800993D0;
extern volatile u_long *D_800993E8;
extern u_char *D_800993EC;
extern char D_80013B14[];


/*
 * Low-level DMA-channel transfer helper. Spins on the channel's CHCR busy bit
 * (register window at 0x1F801088 + ch*0x10, timing out after 0x10000 polls),
 * then programs MADR/BCR/CHCR to start a transfer of `count` blocks of `size`
 * words. `mode` selects between the block-mode / linked-list setups.
 */
void CD_dmastart(long ch, u_long madr, u_long count, u_long size, u_long chcrVal, u_char mode) asm("func_8006DB74");
void CD_dmastart(long ch, u_long madr, u_long count, u_long size, u_long chcrVal, u_char mode) {
    volatile long dummy;
    long i;
    volatile u_long *p;
    register u_char *dptr asm("$3");
    register volatile u_long *dp asm("$4");

    i = 0;
    while (*(volatile u_long *)(0x1F801088 + (ch << 4)) & 0x01000000) {
        if (i == 0x10000) {
            GameDebugPrintf(D_80013B14, *(volatile u_long *)(0x1F801088 + (ch << 4)));
            break;
        }
        i++;
    }

    if (mode == 1) {
        register long bv asm("$4");
        dptr = D_800993EC;
        bv = dptr[2];
        dptr[2] = bv | (1 << ch);
    } else {
        register long bv asm("$4");
        dptr = D_800993EC;
        bv = dptr[2];
        dptr[2] = bv & ~(1 << ch);
    }

    dummy = *(volatile u_long *)D_800993EC;
    asm volatile("");
    {
        register long dv asm("$6");
        long bit;

        dv = ch * 4;
        bit = 1 << (dv + 3);
        asm volatile("");
        p = (volatile u_long *)(0x1F801080 + (ch << 4));
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
