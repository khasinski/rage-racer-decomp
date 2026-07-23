#include "common.h"

// Fixed-point blend in 0..10000 scale.
s32 func_80046598(s32 arg0, s32 arg1) {
    s32 initial;
    s32 value;
    s32 doubled;

    initial = 0x2710 - arg0;
    value = initial;
    doubled = arg0 * 2;
    value = (value * doubled) / 10000;
    return ((value * arg1) + (arg0 * arg0)) / 10000;
}
