#include <sys/types.h>

#include "common.h"

extern volatile u_short *g_SpuRegBase;
extern u_short g_SpuTransferStartAddr;
extern long g_SpuWaitCount;
extern u_char g_SpuTimeoutFmt[];
extern u_char g_SpuTimeoutMsgWrdy[];
extern u_char g_SpuTimeoutMsgDmaf[];


void _spu_writeByIO(u_short *addr, u_long size) {
    volatile long di, dj;
    u_short stat0;
    u_short *paddr = addr;
    u_short saved;
    register u_short ctrl asm("$4");
    register u_short cmasked asm("$2");
    long chunk;
    long k;

#define SPU_DELAY()                           \
    for (dj = 13, di = 0; di < 240; di++) {   \
        dj = dj * 3;                          \
    }

    stat0 = g_SpuRegBase[0xD7];
    g_SpuRegBase[0xD3] = g_SpuTransferStartAddr;
    saved = stat0 & 0x7ff;
    SPU_DELAY();

    if (size != 0) {
        do {
            chunk = (size < 65) ? size : 64;
            for (k = 0; k < chunk; k += 2) {
                g_SpuRegBase[0xD4] = *paddr++;
            }
            ctrl = g_SpuRegBase[0xD5];
            cmasked = ctrl & 0xffcf;
            asm("" : "=r"(cmasked) : "0"(cmasked));
            ctrl = cmasked | 0x10;
            g_SpuRegBase[0xD5] = ctrl;

            SPU_DELAY();

            g_SpuWaitCount = 0;
            while (g_SpuRegBase[0xD7] & 0x400) {
                if (++g_SpuWaitCount >= 5001) {
                    DebugPrintf(g_SpuTimeoutFmt, g_SpuTimeoutMsgWrdy);
                    break;
                }
            }

            SPU_DELAY();
            SPU_DELAY();

            size -= chunk;
        } while (size != 0);
    }

    ctrl = g_SpuRegBase[0xD5];
    g_SpuRegBase[0xD5] = ctrl & 0xffcf;
    g_SpuWaitCount = 0;
    while ((g_SpuRegBase[0xD7] & 0x7ff) != saved) {
        if (++g_SpuWaitCount >= 5001) {
            DebugPrintf(g_SpuTimeoutFmt, g_SpuTimeoutMsgDmaf);
            break;
        }
    }
}
