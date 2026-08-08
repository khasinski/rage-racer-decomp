#include <sys/types.h>
#include <stdio.h>

#include "common.h"
#include "psyq/spu_internal.h"

void _spu_writeByIO(u_short *addr, u_long size) {
    volatile long di, dj;
    u_short stat0;
    u_short *paddr = addr;
    u_short saved;
    u_short ctrl;
    long chunk;
    long k;

#define SPU_DELAY()                           \
    for (dj = 13, di = 0; di < 240; di++) {   \
        dj = dj * 3;                          \
    }

    stat0 = g_SpuRegBase->raw[0xD7];
    g_SpuRegBase->raw[0xD3] = g_SpuTransferStartAddr;
    saved = stat0 & 0x7ff;
    SPU_DELAY();

    if (size != 0) {
        do {
            chunk = (size < 65) ? size : 64;
            for (k = 0; k < chunk; k += 2) {
                g_SpuRegBase->raw[0xD4] = *paddr++;
            }
            ctrl = g_SpuRegBase->raw[0xD5];
            ctrl &= 0xffcf;
            ctrl |= 0x10;
            g_SpuRegBase->raw[0xD5] = ctrl;

            SPU_DELAY();

            g_SpuWaitCount = 0;
            while (g_SpuRegBase->raw[0xD7] & 0x400) {
                if (++g_SpuWaitCount >= 5001) {
                    printf(g_SpuTimeoutFmt, g_SpuTimeoutMsgWrdy);
                    break;
                }
            }

            SPU_DELAY();
            SPU_DELAY();

            size -= chunk;
        } while (size != 0);
    }

    ctrl = g_SpuRegBase->raw[0xD5];
    ctrl &= 0xffcf;
    g_SpuRegBase->raw[0xD5] = ctrl;
    g_SpuWaitCount = 0;
    while ((g_SpuRegBase->raw[0xD7] & 0x7ff) != saved) {
        if (++g_SpuWaitCount >= 5001) {
            printf(g_SpuTimeoutFmt, g_SpuTimeoutMsgDmaf);
            break;
        }
    }
}
