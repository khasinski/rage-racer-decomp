#include <sys/types.h>

#include "common.h"

extern long g_CdCommandNeedsSetloc[] asm("D_80098FBC");
extern long g_CdSyncCallback asm("D_8009903C");
extern u_char g_CdStatusByte asm("D_8009904C");

long CD_cw(long arg0, void *arg1, long arg2, long arg3) asm("func_8006B620");

/*
 * Core CD command sender with retry: issues command `arg0` (low byte) with the
 * parameter bytes at `arg1` and result flags `arg2`, retrying up to 3 times.
 * Saves/restores the CD mode D_8009903C around the call. Heavily register-
 * pinned to match; the C identifiers may be renamed but the asm("$N") pins and
 * offsets must not change.
 */
long CdControl(long com, void *param, long result) asm("func_8006A5A4");
long CdControl(long arg0, void *arg1, long arg2) {
    void *arg;
    long arg2Reg;
    register long cmd asm("$20");
    long retries;
    long command;
    long *base;
    long savedMode;
    long offset;
    long *commandState;
    long result;

    arg = arg1;
    arg2Reg = arg2;
    cmd = arg0;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
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
            if (CD_cw(2, arg, arg2Reg, 0) != 0) {
                continue;
            }
        }

        g_CdSyncCallback = savedMode;
        if (CD_cw(cmd & 0xFF, arg, arg2Reg, 0) == 0) {
            goto done;
        }

    } while (--retries != -1);

    g_CdSyncCallback = savedMode;
    result = -1;

done:
    return result + 1;
}

long CdControlF(long arg0, void *arg1) asm("func_8006A6DC");
long CdControlF(long arg0, void *arg1) {
    void *arg;
    long cmd;
    long retries;
    long command;
    long savedMode;
    long *base;
    long offset;
    long *commandState;
    long result;

    arg = arg1;
    cmd = arg0;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
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
        if (CD_cw(cmd & 0xFF, arg, 0, 1) == 0) {
            goto done;
        }

    } while (--retries != -1);

    g_CdSyncCallback = savedMode;
    result = -1;

done:
    return result + 1;
}

extern long g_CdCommandNeedsSetloc[] asm("D_80098FBC");
extern long g_CdSyncCallback asm("D_8009903C");
extern u_char g_CdStatusByte asm("D_8009904C");

long CD_cw(long arg0, void *arg1, long arg2, long arg3) asm("func_8006B620");
long CD_sync(long arg0, long arg1) asm("func_8006B0D4");

long CdControlB(long arg0, void *arg1, long arg2) asm("func_8006A808");
long CdControlB(long arg0, void *arg1, long arg2) {
    void *arg;
    long arg2Reg;
    register long cmd asm("$20");
    long retries;
    long command;
    long savedMode;
    long *base;
    long offset;
    long *commandState;
    long result;
    long zero;

    arg = arg1;
    arg2Reg = arg2;
    cmd = arg0;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
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
            if (CD_cw(2, arg, arg2Reg, 0) != 0) {
                continue;
            }
        }

        g_CdSyncCallback = savedMode;
        if (CD_cw(cmd & 0xFF, arg, arg2Reg, 0) == 0) {
            result = 0;
            goto done;
        }

    } while (--retries != -1);

    g_CdSyncCallback = savedMode;
    asm("" : "=r"(result));

done:
    zero = 0;
    if (result == 0) {
        result = CD_sync(zero, arg2Reg) == 2;
    } else {
        result = 0;
    }
    return result;
}
