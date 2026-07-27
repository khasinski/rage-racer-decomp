#include <sys/types.h>

#include "common.h"

extern long g_CdCommandNeedsSetloc[] asm("D_80098FBC");
extern long g_CdSyncCallback asm("D_8009903C");
extern u_char g_CdStatusByte asm("D_8009904C");

long func_8006B620(long arg0, void *arg1, long arg2, long arg3);

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
    /* This pin is load-bearing: removing it changes .text. */
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
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
    base = g_CdCommandNeedsSetloc;
    savedMode = g_CdSyncCallback;
    offset = command << 2;
    commandState = (long *)(offset + (long)base);
    result = 0;

    do {
        g_CdSyncCallback = 0;
        if (command != 1) {
            if (g_CdStatusByte & 0x10) {
                func_8006B620(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (func_8006B620(2, arg, arg2Reg, 0) != 0) {
                goto retry;
            }
        }

        g_CdSyncCallback = savedMode;
        if (func_8006B620(cmd & 0xFF, arg, arg2Reg, 0) == 0) {
            goto done;
        }

retry:
        retries--;
    } while (retries != -1);

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
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(command) : "0"(command));
    base = g_CdCommandNeedsSetloc;
    savedMode = g_CdSyncCallback;
    offset = command << 2;
    commandState = (long *)(offset + (long)base);
    result = 0;

    do {
        g_CdSyncCallback = 0;
        if (command != 1) {
            if (g_CdStatusByte & 0x10) {
                func_8006B620(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (func_8006B620(2, arg, 0, 0) != 0) {
                goto retry;
            }
        }

        g_CdSyncCallback = savedMode;
        if (func_8006B620(cmd & 0xFF, arg, 0, 1) == 0) {
            goto done;
        }

retry:
        retries--;
    } while (retries != -1);

    g_CdSyncCallback = savedMode;
    result = -1;

done:
    return result + 1;
}

extern long g_CdCommandNeedsSetloc[] asm("D_80098FBC");
extern long g_CdSyncCallback asm("D_8009903C");
extern u_char g_CdStatusByte asm("D_8009904C");

long func_8006B620(long arg0, void *arg1, long arg2, long arg3);
long func_8006B0D4(long arg0, long arg1);

long CdControlB(long arg0, void *arg1, long arg2) asm("func_8006A808");
long CdControlB(long arg0, void *arg1, long arg2) {
    void *arg;
    long arg2Reg;
    /* This pin is load-bearing: removing it changes .text. */
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
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
    base = g_CdCommandNeedsSetloc;
    savedMode = g_CdSyncCallback;
    offset = command << 2;
    commandState = (long *)(offset + (long)base);
    result = 0;

    do {
        g_CdSyncCallback = 0;
        if (command != 1) {
            if (g_CdStatusByte & 0x10) {
                func_8006B620(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (func_8006B620(2, arg, arg2Reg, 0) != 0) {
                goto retry;
            }
        }

        g_CdSyncCallback = savedMode;
        if (func_8006B620(cmd & 0xFF, arg, arg2Reg, 0) == 0) {
            result = 0;
            goto done;
        }

retry:
        retries--;
    } while (retries != -1);

    g_CdSyncCallback = savedMode;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(result));

done:
    zero = 0;
    if (result == 0) {
        result = func_8006B0D4(zero, arg2Reg) == 2;
    } else {
        result = 0;
    }
    return result;
}
