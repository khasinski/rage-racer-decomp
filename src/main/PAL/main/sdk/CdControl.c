#include <sys/types.h>

#include "common.h"

extern long g_CdCommandNeedsSetloc[];
extern long g_CdSyncCallback;
extern u_char g_CdStatusByte;

long CD_cw(long arg0, void *arg1, long arg2, long arg3);

/*
 * Core CD command sender with retry: issues command `arg0` (low byte) with the
 * parameter bytes at `arg1` and result flags `arg2`, retrying up to 3 times.
 * Saves/restores the CD mode g_CdSyncCallback around the call. Heavily register-
 * pinned to match; the C identifiers may be renamed but the asm("$N") pins and
 * offsets must not change.
 */
long CdControl(long com, void *param, long result);
long CdControl(long com, void *param, long result) {
    void *arg;
    long arg2Reg;
    register long cmd asm("$20");
    long retries;
    long command;
    long *base;
    long savedMode;
    long offset;
    long *commandState;
    long status;

    arg = param;
    arg2Reg = result;
    cmd = com;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = (u8)cmd;
    base = g_CdCommandNeedsSetloc;
    savedMode = g_CdSyncCallback;
    offset = command * 4;
    commandState = (long *)(offset + (long)base);
    status = 0;

    do {
        g_CdSyncCallback = 0;
        if (command != 1) {
            if (g_CdStatusByte & 0x10) {
                CD_cw(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (CD_cw(2, arg, arg2Reg, 0) != 0) {
                continue;
            }
        }

        g_CdSyncCallback = savedMode;
        if (CD_cw((u8)cmd, arg, arg2Reg, 0) == 0) {
            goto done;
        }

    } while (--retries != -1);

    g_CdSyncCallback = savedMode;
    status = -1;

done:
    return status + 1;
}

long CdControlF(long com, void *param) {
    void *arg;
    long cmd;
    long retries;
    long command;
    long savedMode;
    long *base;
    long offset;
    long *commandState;
    long result;

    arg = param;
    cmd = com;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = (u8)cmd;
    asm("" : "=r"(command) : "0"(command));
    base = g_CdCommandNeedsSetloc;
    savedMode = g_CdSyncCallback;
    offset = command * 4;
    commandState = (long *)(offset + (long)base);
    result = 0;

    do {
        g_CdSyncCallback = 0;
        if (command != 1) {
            if (g_CdStatusByte & 0x10) {
                CD_cw(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (CD_cw(2, arg, 0, 0) != 0) {
                continue;
            }
        }

        g_CdSyncCallback = savedMode;
        if (CD_cw((u8)cmd, arg, 0, 1) == 0) {
            goto done;
        }

    } while (--retries != -1);

    g_CdSyncCallback = savedMode;
    result = -1;

done:
    return result + 1;
}


long CD_sync(long arg0, long arg1);

long CdControlB(long com, void *param, long result) {
    void *arg;
    long arg2Reg;
    register long cmd asm("$20");
    long retries;
    long command;
    long savedMode;
    long *base;
    long offset;
    long *commandState;
    long status;
    long zero;

    arg = param;
    arg2Reg = result;
    cmd = com;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = (u8)cmd;
    base = g_CdCommandNeedsSetloc;
    savedMode = g_CdSyncCallback;
    offset = command * 4;
    commandState = (long *)(offset + (long)base);
    status = 0;

    do {
        g_CdSyncCallback = 0;
        if (command != 1) {
            if (g_CdStatusByte & 0x10) {
                CD_cw(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (CD_cw(2, arg, arg2Reg, 0) != 0) {
                continue;
            }
        }

        g_CdSyncCallback = savedMode;
        if (CD_cw((u8)cmd, arg, arg2Reg, 0) == 0) {
            status = 0;
            goto done;
        }

    } while (--retries != -1);

    g_CdSyncCallback = savedMode;
    asm("" : "=r"(status));

done:
    zero = 0;
    if (status == 0) {
        status = CD_sync(zero, arg2Reg) == 2;
    } else {
        status = 0;
    }
    return status;
}
