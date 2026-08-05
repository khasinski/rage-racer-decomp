#include <sys/types.h>

#include "common.h"

extern volatile u_char *D_800993D0;
extern volatile u_long *g_DmaDpcr;
extern u_char *g_DmaDicr;
extern char D_80013B14[];


/*
 * Low-level DMA-channel transfer helper. Spins on the channel's CHCR busy bit
 * (register window at 0x1F801088 + ch*0x10, timing out after 0x10000 polls),
 * then programs MADR/BCR/CHCR to start a transfer of `count` blocks of `size`
 * words. `mode` selects between the block-mode / linked-list setups.
 */
void CD_dmastart(long ch, u_long madr, u_long count, u_long size, u_long chcrVal, u_char mode) {
    volatile long dummy;
    long i;
    volatile u_long *p;
    u_char *dptr;
    volatile u_long *dp;

    i = 0;
    while (*(volatile u_long *)(0x1F801088 + (ch << 4)) & 0x01000000) {
        if (i == 0x10000) {
            DebugPrintf(D_80013B14, *(volatile u_long *)(0x1F801088 + (ch << 4)));
            break;
        }
        i++;
    }

    if (mode == 1) {
        register long bv asm("$4");
        dptr = g_DmaDicr;
        bv = dptr[2];
        dptr[2] = bv | (1 << ch);
    } else {
        register long bv asm("$4");
        dptr = g_DmaDicr;
        bv = dptr[2];
        dptr[2] = bv & ~(1 << ch);
    }

    dummy = *(volatile u_long *)g_DmaDicr;
    asm volatile("");
    {
        register long dv asm("$6");
        long bit;

        dv = ch * 4;
        bit = 1 << (dv + 3);
        asm volatile("");
        p = (volatile u_long *)(0x1F801080 + (ch << 4));
        dp = g_DmaDpcr;
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
