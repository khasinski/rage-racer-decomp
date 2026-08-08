#include <sys/types.h>
#include <stdio.h>

#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"

extern long g_CdCommandNames[];
extern volatile u_char g_CdSyncStatus[];
extern long g_CdIntrNames[];
extern long g_CdTimeoutDeadline;
extern void *D_8009BB10;
extern char D_8001391C[];
extern char D_80013814[];
extern char D_80013824[];

/* Waits for the CD data transfer to finish, with a 0x3C0-vblank / 0x3C0000-
 * spin watchdog that prints "CD timeout: " and flushes. Named by the same
 * self-naming idiom as CD_sync / CD_ready / CD_cw: it stores its own name
 * string D_8001391C = "CD_datasync" into the D_8009BB10 slot the timeout
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
    D_8009BB10 = D_8001391C;
    do {
        long status;
        if (VSync(-1) > g_CdTimeoutDeadline || g_CdTimeoutCounter++ > 0x3C0000) {
            puts(D_80013814);
            printf((u8 *)D_80013824,
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
