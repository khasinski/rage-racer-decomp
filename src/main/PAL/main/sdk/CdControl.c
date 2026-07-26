#include "common.h"

extern s32 D_80098FBC[];
extern s32 D_8009903C;
extern u8 D_8009904C;

s32 func_8006B620(s32 arg0, void *arg1, s32 arg2, s32 arg3);

/*
 * Core CD command sender with retry: issues command `arg0` (low byte) with the
 * parameter bytes at `arg1` and result flags `arg2`, retrying up to 3 times.
 * Saves/restores the CD mode D_8009903C around the call. Heavily register-
 * pinned to match; the C identifiers may be renamed but the asm("$N") pins and
 * offsets must not change.
 */
s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");
s32 CdControl(s32 arg0, void *arg1, s32 arg2) {
    register void *arg asm("$17");
    register s32 arg2Reg asm("$18");
    register s32 cmd asm("$20");
    register s32 retries asm("$16");
    register s32 command asm("$19");
    register s32 *base asm("$3");
    register s32 savedMode asm("$21");
    register s32 offset asm("$2");
    register s32 *commandState asm("$22");
    register s32 result asm("$23");

    arg = arg1;
    arg2Reg = arg2;
    cmd = arg0;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
    asm("" : "=r"(command) : "0"(command));
    base = D_80098FBC;
    asm("" : "=r"(base) : "0"(base));
    savedMode = D_8009903C;
    asm("" : "=r"(savedMode) : "0"(savedMode));
    offset = command << 2;
    commandState = (s32 *)(offset + (s32)base);
    result = 0;

    do {
        D_8009903C = 0;
        if (command != 1) {
            if (D_8009904C & 0x10) {
                func_8006B620(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (func_8006B620(2, arg, arg2Reg, 0) != 0) {
                goto retry;
            }
        }

        D_8009903C = savedMode;
        if (func_8006B620(cmd & 0xFF, arg, arg2Reg, 0) == 0) {
            goto done;
        }

retry:
        retries--;
    } while (retries != -1);

    asm("");
    D_8009903C = savedMode;
    asm("" : "=r"(retries) : "0"(retries));
    result = -1;

done:
    return result + 1;
}

s32 CdControlF(s32 arg0, void *arg1) asm("func_8006A6DC");
s32 CdControlF(s32 arg0, void *arg1) {
    register void *arg asm("$17");
    register s32 cmd asm("$19");
    register s32 retries asm("$16");
    register s32 command asm("$18");
    register s32 savedMode asm("$20");
    register s32 *base asm("$3");
    register s32 offset asm("$2");
    register s32 *commandState asm("$21");
    register s32 result asm("$22");

    arg = arg1;
    cmd = arg0;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
    asm("" : "=r"(command) : "0"(command));
    base = D_80098FBC;
    asm("" : "=r"(base) : "0"(base));
    savedMode = D_8009903C;
    asm("" : "=r"(savedMode) : "0"(savedMode));
    offset = command << 2;
    commandState = (s32 *)(offset + (s32)base);
    result = 0;

    do {
        D_8009903C = 0;
        if (command != 1) {
            if (D_8009904C & 0x10) {
                func_8006B620(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (func_8006B620(2, arg, 0, 0) != 0) {
                goto retry;
            }
        }

        D_8009903C = savedMode;
        if (func_8006B620(cmd & 0xFF, arg, 0, 1) == 0) {
            goto done;
        }

retry:
        retries--;
    } while (retries != -1);

    asm("");
    D_8009903C = savedMode;
    asm("" : "=r"(retries) : "0"(retries));
    result = -1;

done:
    return result + 1;
}

extern s32 D_80098FBC[];
extern s32 D_8009903C;
extern u8 D_8009904C;

s32 func_8006B620(s32 arg0, void *arg1, s32 arg2, s32 arg3);
s32 func_8006B0D4(s32 arg0, s32 arg1);

s32 CdControlB(s32 arg0, void *arg1, s32 arg2) asm("func_8006A808");
s32 CdControlB(s32 arg0, void *arg1, s32 arg2) {
    register void *arg asm("$17");
    register s32 arg2Reg asm("$18");
    register s32 cmd asm("$20");
    register s32 retries asm("$16");
    register s32 command asm("$19");
    register s32 savedMode asm("$21");
    register s32 *base asm("$3");
    register s32 offset asm("$2");
    register s32 *commandState asm("$22");
    register s32 result asm("$2");
    register s32 zero asm("$4");

    arg = arg1;
    arg2Reg = arg2;
    cmd = arg0;
    asm("" : "=r"(cmd) : "0"(cmd));
    retries = 3;
    command = cmd & 0xFF;
    asm("" : "=r"(command) : "0"(command));
    base = D_80098FBC;
    asm("" : "=r"(base) : "0"(base));
    savedMode = D_8009903C;
    asm("" : "=r"(savedMode) : "0"(savedMode));
    offset = command << 2;
    commandState = (s32 *)(offset + (s32)base);
    result = 0;

    do {
        D_8009903C = 0;
        if (command != 1) {
            if (D_8009904C & 0x10) {
                func_8006B620(1, 0, 0, 0);
            }
        }

        if (arg != 0 && *commandState != 0) {
            if (func_8006B620(2, arg, arg2Reg, 0) != 0) {
                goto retry;
            }
        }

        D_8009903C = savedMode;
        if (func_8006B620(cmd & 0xFF, arg, arg2Reg, 0) == 0) {
            result = 0;
            goto done;
        }

retry:
        retries--;
    } while (retries != -1);

    asm("");
    D_8009903C = savedMode;
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
