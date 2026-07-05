#include "common.h"

void SpuVmSeKeyOn(s32 arg0, s16 arg1, s16 arg2, u16 arg3, u16 arg4, u16 arg5) asm("func_80076350");
void SpuVmSeKeyOff(s32 arg0, s16 arg1, s16 arg2, u16 arg3) asm("func_80076940");

void SsUtKeyOn(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4, u16 arg5) asm("func_80076B30");
void SsUtKeyOff(s32 arg0, s32 arg1, s32 arg2) asm("func_80076C1C");
void SsUtKeyOnV(void) asm("func_80076C50");

void SsUtKeyOn(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4, u16 arg5) {
    u16 base;
    s32 balance;
    register s32 quotient asm("$2");

    if (arg4 == arg5) {
        balance = 0x40;
        base = arg4;
    } else if (arg5 < arg4) {
        quotient = (arg5 << 6) / arg4;
        base = arg4;
        balance = quotient;
    } else {
        quotient = (arg4 << 6) / arg5;
        base = arg5;
        balance = 0x7F - quotient;
    }

    SpuVmSeKeyOn(0x21, (s16)arg0, (s16)arg1, (u16)arg2, base, (u16)balance);
}

void SsUtKeyOff(s32 arg0, s32 arg1, s32 arg2) {
    SpuVmSeKeyOff(0x21, arg0, arg1, arg2);
}

void SsUtKeyOnV(void) {
}
