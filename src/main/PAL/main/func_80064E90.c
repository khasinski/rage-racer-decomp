#include "common.h"

void SetSemiTrans(u8 *arg0, s32 arg1) asm("func_80064E90");

void SetSemiTrans(u8 *arg0, s32 arg1) {
    s32 value;

    if (arg1 != 0) {
        value = arg0[7] | 2;
    } else {
        value = arg0[7] & 0xFD;
    }
    arg0[7] = value;
}

void SetShadeTex(u8 *arg0, s32 arg1) asm("func_80064EB8");

void SetShadeTex(u8 *arg0, s32 arg1) {
    s32 value;

    if (arg1 != 0) {
        value = arg0[7] | 1;
    } else {
        value = arg0[7] & 0xFE;
    }
    arg0[7] = value;
}
