#include <sys/types.h>
#include <stdio.h>

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
extern u_char g_CdDataEndResult[];
extern long g_CdTimeoutDeadline;
extern char *g_CdTimeoutName;
extern char g_MsgCdTimeout[];
extern char g_FmtCdTimeoutState[];
extern char g_MsgCdReadyName[];

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

long CD_ready(long mode, u_char *result) {
    long savedStatus;
    long interrupt;
    long status;
    long alarmStatus;

    g_CdTimeoutDeadline = VSync(-1) + 0x3C0;
    g_CdTimeoutCounter = 0;
    g_CdTimeoutName = g_MsgCdReadyName;

    for (;;) {
        if (g_CdTimeoutDeadline < VSync(-1) ||
            g_CdTimeoutCounter++ > 0x3C0000) {
            puts(g_MsgCdTimeout);
            printf((u8 *)g_FmtCdTimeoutState, ((CdAlarm *)&g_CdTimeoutDeadline)->name,
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
