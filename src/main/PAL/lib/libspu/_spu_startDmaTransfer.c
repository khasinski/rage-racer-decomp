#include "psyq/spu.h"

/* SPU hardware, from the data segment: register file base 0x1F801C00, DMA
 * channel 4 (SPU) MADR/BCR/CHCR 0x1F8010C0/C4/C8, SPU_DELAY 0x1F801014. */
extern volatile u_short *g_SpuRegBase asm("D_8009AB7C");
extern volatile u_long *g_SpuDmaMadr asm("D_8009AB80");
extern volatile u_long *g_SpuDmaBcr asm("D_8009AB84");
extern volatile u_long *g_SpuDmaChcr asm("D_8009AB88");
extern volatile u_long *g_SpuDelayReg asm("D_8009AB90");
extern long g_SpuTransferIsRead asm("D_8009ABC8");

void _spu_startDmaTransfer(u_long arg0, u_short arg1, u_long arg2) {
    u_long addr = arg0;
    volatile long i;
    volatile long delay;
    u_short cnt;
    u_long value;

    g_SpuRegBase[0xD3] = arg1;

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    cnt = g_SpuRegBase[0xD5];
    g_SpuRegBase[0xD5] = cnt | 0x30;

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    value = *g_SpuDelayReg;
    value &= 0xF0FFFFFF;
    value |= 0x22000000;
    *g_SpuDelayReg = value;

    *g_SpuDmaMadr = addr;
    *g_SpuDmaBcr = (arg2 << 0x10) | 0x10;
    g_SpuTransferIsRead = 1;
    *g_SpuDmaChcr = 0x01000200;
}
