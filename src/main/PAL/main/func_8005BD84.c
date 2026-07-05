#include "common.h"

void func_80043134(s32 arg0);
void func_8005E834(s32 arg0);

void func_8005BD84(s32 arg0) {
    register s32 value asm("$16");

    value = arg0;
    asm("" : "=r"(value) : "0"(value));
    if (value < 0) {
        goto negative;
    }
    if (arg0 < 0x10) {
        goto call;
    }

    arg0 = 0xF;
    goto call;

negative:
    arg0 = 0;

call:
    value = arg0;
    func_80043134(arg0);
    func_8005E834(value);
}

asm(".globl func_8005BDB0\nfunc_8005BDB0 = func_8005BD84 + 0x2C");
