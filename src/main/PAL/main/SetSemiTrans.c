#include "common.h"

void func_80064E90(u8 *arg0, s32 arg1) {
    s32 value;

    if (arg1 != 0) {
        value = arg0[7] | 2;
    } else {
        value = arg0[7] & 0xFD;
    }
    arg0[7] = value;
}

void func_80064EB8(u8 *arg0, s32 arg1) {
    s32 value;

    if (arg1 != 0) {
        value = arg0[7] | 1;
    } else {
        value = arg0[7] & 0xFE;
    }
    arg0[7] = value;
}
