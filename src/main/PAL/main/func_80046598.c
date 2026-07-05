#include "common.h"

// Fixed-point blend in 0..10000 scale.
s32 func_80046598(s32 arg0, s32 arg1) {
    register s32 value asm("$2");
    register s32 doubled asm("$3");

    value = 0x2710 - arg0;
    doubled = arg0 * 2;
    value = (value * doubled) / 10000;
    return ((value * arg1) + (arg0 * arg0)) / 10000;
}
