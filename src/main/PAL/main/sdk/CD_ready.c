#include <sys/types.h>

#include "common.h"
#include "psyq/cd.h"




extern char *g_CdCommandNames[] asm("D_80099060");
extern char *g_CdIntrNames[] asm("D_800990E0");
extern CdCallback g_CdSyncCallback asm("D_8009903C");
extern CdCallback g_CdReadyCallback asm("D_80099040");
extern u_char g_CdLastCommand asm("D_8009905D");
extern volatile u_char *g_CdReg0 asm("D_80099300");
extern volatile CdIntr g_CdSyncStatus asm("D_80099318");
extern u_char g_CdSyncResult[] asm("D_8009BAF0");
extern u_char g_CdReadyResult[] asm("D_8009BAF8");
extern u_char g_CdDataEndResult[] asm("D_8009BB00");
extern long g_CdTimeoutDeadline asm("D_8009BB08");
extern long g_CdTimeoutCounter asm("D_8009BB0C");
extern char *D_8009BB10;
extern char D_80013814[];
extern char D_80013824[];
extern char D_800138A4[];

void LibcPutString(char *text) asm("func_80063C38");
long CdReadInterruptStatus(void) asm("func_8006AB5C");
void CD_flush(void) asm("func_8006BAF0");
long VSync(long mode) asm("func_8006DD30");
long GetKernelStatus(void) asm("func_8006E088");

static __inline__ void copy8(u_char *dst, u_char *src) {
    long count;
    long end;

    if (dst == 0) {
        return;
    }

    count = 7;
    end = -1;
    do {
        *dst++ = *src++;
    } while (--count != end);
}

long CD_ready(long mode, u_char *result) asm("func_8006B354");
long CD_ready(long mode, u_char *result) {
    long savedStatus;
    long interrupt;
    long status;
    long alarmStatus;

    g_CdTimeoutDeadline = VSync(-1) + 0x3C0;
    g_CdTimeoutCounter = 0;
    D_8009BB10 = D_800138A4;

    for (;;) {
        if (g_CdTimeoutDeadline < VSync(-1) ||
            g_CdTimeoutCounter++ > 0x3C0000) {
            LibcPutString(D_80013814);
            DebugPrintf(D_80013824, ((CdAlarm *)&g_CdTimeoutDeadline)->name,
                          g_CdCommandNames[g_CdLastCommand],
                          g_CdIntrNames[g_CdSyncStatus.sync],
                          g_CdIntrNames[g_CdSyncStatus.ready]);
            CD_flush();
            alarmStatus = -1;
        } else {
            alarmStatus = 0;
        }

        if (alarmStatus != 0) {
            return -1;
        }

        if (GetKernelStatus()) {
            savedStatus = *g_CdReg0 % 4;
            while ((interrupt = CdReadInterruptStatus()) != 0) {
                if ((interrupt & 4) != 0 && g_CdReadyCallback != 0) {
                    g_CdReadyCallback(g_CdSyncStatus.ready, g_CdReadyResult);
                }
                if ((interrupt & 2) != 0 && g_CdSyncCallback != 0) {
                    g_CdSyncCallback(g_CdSyncStatus.sync, g_CdSyncResult);
                }
            }
            *g_CdReg0 = savedStatus;
        }

        status = g_CdSyncStatus.command;
        if (status != 0) {
            g_CdSyncStatus.command = 0;
            copy8(result, g_CdDataEndResult);
            return status;
        }

        status = g_CdSyncStatus.ready;
        if (status != 0) {
            g_CdSyncStatus.ready = 0;
            copy8(result, g_CdReadyResult);
            return status;
        }

        if (mode != 0) {
            return 0;
        }
    }
}
