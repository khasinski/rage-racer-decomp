#include <sys/types.h>
#include <stdio.h>

#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"

extern long g_CdCommandNames[];
extern volatile u_char g_CdSyncStatus[];
extern long g_CdIntrNames[];
extern long g_CdTimeoutDeadline;
extern void *g_CdTimeoutName;
extern char g_MsgCdDataSync[];
extern char g_MsgCdTimeout[];
extern char g_FmtCdTimeoutState[];

/* Waits for the CD data transfer to finish, with a 0x3C0-vblank / 0x3C0000-
 * spin watchdog that prints "CD timeout: " and flushes. Named by the same
 * self-naming idiom as CD_sync / CD_ready / CD_cw: it stores its own name
 * string g_MsgCdDataSync = "CD_datasync" into the g_CdTimeoutName slot the timeout
 * message prints as its first %s. arg 0 blocks, non-zero polls. */
long CD_datasync(long arg) {
    long *b60;
    volatile u_char *b318;
    long *bE0;
    long result;

    g_CdTimeoutDeadline = VSync(-1) + 0x3C0;
    b60 = g_CdCommandNames;
    b318 = g_CdSyncStatus;
    bE0 = g_CdIntrNames;
    g_CdTimeoutCounter = 0;
    g_CdTimeoutName = g_MsgCdDataSync;
    do {
        long status;
        if (VSync(-1) > g_CdTimeoutDeadline || g_CdTimeoutCounter++ > 0x3C0000) {
            puts(g_MsgCdTimeout);
            printf((u8 *)g_FmtCdTimeoutState,
                            ((CdAlarm *)&g_CdTimeoutDeadline)->name,
                            b60[g_CdLastCommand],
                            bE0[b318[0]],
                            bE0[b318[1]]);
            CD_flush();
            status = -1;
        } else {
            status = 0;
        }
        if (status != 0) {
            result = -1;
            return result;
        }
        if ((*g_CdDmaChcr & 0x1000000) == 0) {
            result = 0;
            goto done;
        }
    } while (arg == 0);
    result = 1;
done:
    return result;
}
