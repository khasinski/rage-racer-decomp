#include <sys/types.h>

#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"

extern char *g_CdCommandNames[];
extern char *g_CdIntrNames[];
extern CdCallback g_CdSyncCallback;
extern CdCallback g_CdReadyCallback;
extern volatile CdIntr g_CdSyncStatus;
extern u_char g_CdSyncResult[];
extern u_char g_CdReadyResult[];
extern long g_CdTimeoutDeadline;
extern char *D_8009BB10;
extern char D_80013814[];
extern char D_80013824[];
extern char D_8001389C[];

void LibcPutString(char *text);

static __inline__ void copy8(u_char *dst, u_char *src) {
    register u_char *dstReg asm("$5");
    u_char *srcReg;
    long count;
    long end;

    dstReg = dst;
    srcReg = src;

    if (dstReg == 0) {
        return;
    }

    count = 7;
    end = -1;
    do {
        *dstReg++ = *srcReg++;
    } while (--count != end);
}

long CD_sync(long mode, u_char *result) {
    register long modeReg asm("$21");
    register u_char *resultReg asm("$22");
    char **statusNames;
    volatile CdIntr *intr;
    u_char *ready;
    long savedStatus;
    long interrupt;
    long alarmStatus;

    modeReg = mode;
    asm("" : "=r"(modeReg) : "0"(modeReg));
    resultReg = result;
    asm("" : "=r"(resultReg) : "0"(resultReg));
    g_CdTimeoutDeadline = VSync(-1) + 0x3C0;
    statusNames = g_CdIntrNames;
    intr = &g_CdSyncStatus;
    ready = (u_char *)&intr->ready;
    g_CdTimeoutCounter = 0;
    D_8009BB10 = D_8001389C;

    for (;;) {
        if (g_CdTimeoutDeadline < VSync(-1) ||
            g_CdTimeoutCounter++ > 0x3C0000) {
            LibcPutString(D_80013814);
            DebugPrintf(D_80013824, ((CdAlarm *)&g_CdTimeoutDeadline)->name,
                          g_CdCommandNames[g_CdLastCommand],
                          statusNames[intr->sync], statusNames[intr->ready]);
            CD_flush();
            alarmStatus = -1;
        } else {
            alarmStatus = 0;
        }

        if (alarmStatus != 0) {
            return -1;
        }

        if (GetKernelStatus()) {
            u_long rawStatus;

            rawStatus = *g_CdReg0;
            savedStatus = rawStatus % 4;
            for (;;) {
                long readyBit;
                s16 syncBit;

                interrupt = CdReadInterruptStatus();
                if (interrupt == 0) {
                    break;
                }
                readyBit = interrupt & 4;
                if (readyBit != 0) {
                    if (g_CdReadyCallback != 0) {
                        g_CdReadyCallback(*ready, g_CdReadyResult);
                    }
                    syncBit = interrupt & 2;
                } else {
                    syncBit = interrupt & 2;
                }
                if (syncBit && g_CdSyncCallback != 0) {
                    g_CdSyncCallback(intr->sync, g_CdSyncResult);
                }
            }
            *g_CdReg0 = savedStatus;
        }

        {
        long sync;
        s8 syncRaw;

        syncRaw = intr->sync;
        asm("" : "=r"(syncRaw) : "0"(syncRaw));
        sync = syncRaw & 0xFF;
        if (sync == 2 || sync == 5) {
            intr->sync = 2;
            copy8(resultReg, (u_char *)g_CdSyncResult);
            asm(".globl func_8006B330\nfunc_8006B330 = CD_sync + 0x25c");
            return sync;
        }
        }

        if (modeReg != 0) {
            return 0;
        }
    }
}
