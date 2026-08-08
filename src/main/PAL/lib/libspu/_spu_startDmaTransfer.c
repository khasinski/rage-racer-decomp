#include "psyq/spu.h"
#include "psyq/spu_internal.h"

/* SPU hardware, from the data segment: file base 0x1F801C00, DMA
 * channel 4 (SPU) MADR/BCR/CHCR 0x1F8010C0/C4/C8, SPU_DELAY 0x1F801014. */

void _spu_startDmaTransfer(u_long ramAddr, u_short spuAddr, u_long words) {
    u_long addr = ramAddr;
    volatile long i;
    volatile long delay;
    u_short cnt;
    u_long value;

    g_SpuRegBase->raw[0xD3] = spuAddr;

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

    cnt = g_SpuRegBase->raw[0xD5];
    g_SpuRegBase->raw[0xD5] = cnt | 0x30;

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
    *g_SpuDmaBcr = (words << 0x10) | 0x10;
    g_SpuTransferIsRead = 1;
    *g_SpuDmaChcr = 0x01000200;
}
