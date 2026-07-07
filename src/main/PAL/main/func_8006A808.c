#include "common.h"


extern s32 D_80098FBC[];
extern s32 D_8009903C;
extern u8 D_8009904C;

s32 func_8006B620(s32 arg0, void *arg1, s32 arg2, s32 arg3);
s32 func_8006B0D4(s32 arg0, s32 arg1);

s32 func_8006A808(s32 arg0, void *arg1, s32 arg2) {
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
    asm("addu %0,%1,%2" : "=r"(commandState) : "r"(offset), "r"(base));
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
