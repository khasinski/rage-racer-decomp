#include <sys/types.h>

#include "common.h"

/*
 * LEFT RAW ON PURPOSE (docs/names.md 17i). This is libcd's interrupt decoder:
 * it reads the 8-byte response FIFO, decodes intr codes 1..5 into D_80099318
 * and the two result buffers, and is drained in a while loop by CD_sync,
 * CD_ready, CD_cw and the IRQ2 handler. Every other libcd internal in this
 * image was pinned because it stores its own name into a trace slot; this one
 * owns only "DiskError: " and "CDROM: unknown intr", neither of which names a
 * function. CD_intr / CD_getintr / CD_status / CD_readIntr are all
 * unfalsifiable here, so no name is asserted.
 *
 * Confirmed under emulation 2026-08-03: across a boot-to-race trace it is the
 * only writer of g_CdSyncStatus, g_CdSyncResult, g_CdReadyStatus and
 * g_CdReadyResult, and reads no global at all. That matches the decoder
 * reading above exactly -- but it still does not pin a Sony name, so the
 * decision to leave it raw stands.
 */

extern volatile u_char *g_CdReg0 asm("D_80099300");
extern volatile u_char *g_CdReg1 asm("D_80099304");
extern volatile u_char *g_CdReg2 asm("D_80099308");
extern volatile u_char *g_CdReg3 asm("D_8009930C");
extern long g_CdDebugLevel asm("D_80099048");
extern u_long g_CdStatusByte asm("D_8009904C");
extern u_long g_CdErrorByte asm("D_80099050");
extern u_long g_CdShellOpenCount asm("D_80099054");
extern u_long g_CdCommandNames[] asm("D_80099060");
extern u_long g_CdCommandHasComplete[] asm("D_80099100");
extern u_long g_CdCommandAckHasStatus[] asm("D_80099200");
extern u_char D_8009905D;
extern volatile u_char g_CdSyncStatus asm("D_80099318");
extern volatile u_char g_CdReadyStatus asm("D_80099319");
extern volatile u_char g_CdDataEndStatus asm("D_8009931A");
extern u_char g_CdSyncResult asm("D_8009BAF0");
extern u_char g_CdReadyResult asm("D_8009BAF8");
extern u_char g_CdDataEndResult asm("D_8009BB00");
extern u_char D_80013840;
extern u_char D_8001384C;
extern u_char D_80013868;
extern u_char D_8001387C;

void func_80063C38();

static __inline__ void copy8(u_char *d, u_char *s) {
    long n;
    if (d == 0) {
        return;
    }
    n = 7;
    do {
        *d++ = *s++;
    } while (--n != -1);
}

long func_8006AB5C(void) {
    volatile u_char mode;
    volatile u_char buf[8];
    long i;
    long flag;
    long v;
    volatile u_char *p;
    volatile u_char *q;

    *g_CdReg0 = 1;
    p = g_CdReg3;
    mode = *p & 7;
    if (mode == 0) {
        return 0;
    }
    flag = 0;
    while (mode != (*p & 7)) {
        mode = *p & 7;
    }
    i = 0;
    q = buf;
    for (; i < 8; i++) {
        if (!(*g_CdReg0 & 0x20)) {
            break;
        }
        *q++ = *g_CdReg1;
    }
    if (i < 8) {
        volatile u_char *r;
        r = (volatile u_char *)(i + (long)buf);
        do {
            *r++ = 0;
        } while ((long)r < (long)&buf[8]);
    }

    *g_CdReg0 = 1;
    *g_CdReg3 = 7;
    *g_CdReg2 = 7;

    if (!(mode == 3 && g_CdCommandAckHasStatus[D_8009905D] == 0)) {
        if (!(g_CdStatusByte & 0x10) && (buf[0] & 0x10)) {
            g_CdShellOpenCount++;
        }
        v = buf[0];
        flag = v & 0x1d;
        g_CdStatusByte = v;
        g_CdErrorByte = buf[1];
    }

    if (mode == 5) {
        func_80063C38(&D_80013840);
        if (g_CdDebugLevel > 0) {
            DebugPrintf(&D_8001384C, g_CdCommandNames[D_8009905D], g_CdStatusByte, g_CdErrorByte);
        }
    }

    switch (mode) {
    case 3:
        if (flag) {
            volatile u_char *sp = &g_CdSyncStatus;
            *sp = 5;
            copy8(&g_CdSyncResult, (u_char *)buf);
            return 2;
        }
        if (g_CdCommandHasComplete[D_8009905D] != 0) {
            volatile u_char *sp = &g_CdSyncStatus;
            *sp = 3;
            copy8(&g_CdSyncResult, (u_char *)buf);
            return 1;
        }
        {
            volatile u_char *sp = &g_CdSyncStatus;
            *sp = 2;
        }
        copy8(&g_CdSyncResult, (u_char *)buf);
        return 2;
    case 2:
        g_CdSyncStatus = flag ? 5 : 2;
        copy8(&g_CdSyncResult, (u_char *)buf);
        return 2;
    case 1:
        if (flag) {
            if (i == 1) {
                flag = 0;
            }
        }
        g_CdReadyStatus = flag ? 5 : 1;
        copy8(&g_CdReadyResult, (u_char *)buf);
        *g_CdReg0 = 0;
        *g_CdReg3 = 0;
        return 4;
    case 4: {
        volatile u_char *sp = &g_CdReadyStatus;
        g_CdDataEndStatus = 4;
        *sp = g_CdDataEndStatus;
        copy8(&g_CdDataEndResult, (u_char *)buf);
        copy8(&g_CdReadyResult, (u_char *)buf);
        return 4;
    }
    case 5: {
        volatile u_char *sp = &g_CdSyncStatus;
        g_CdReadyStatus = 5;
        *sp = g_CdReadyStatus;
        copy8(&g_CdSyncResult, (u_char *)buf);
        copy8(&g_CdReadyResult, (u_char *)buf);
        return 6;
    }
    default:
        func_80063C38(&D_80013868);
        DebugPrintf(&D_8001387C, mode);
        break;
    }
    return 0;
}
