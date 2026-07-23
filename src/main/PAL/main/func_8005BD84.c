#include "common.h"

void func_80043134(s32 arg0);
void func_8005E834(s32 arg0);

void func_8005BD84(s32 arg0) {
    u32 adjusted;
    s32 value;

    value = arg0;
    if (value < 0) {
        goto negative;
    }

    adjusted = arg0;
    adjusted++;
    adjusted--;
    arg0 = adjusted;
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
