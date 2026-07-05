#include "common.h"

void SpuVmSeKeyOff(s32 arg0, s16 arg1, s16 arg2, u16 arg3) asm("func_80076940");

void SsUtKeyOff(s32 arg0, s32 arg1, s32 arg2) asm("func_80076C1C");

void SsUtKeyOff(s32 arg0, s32 arg1, s32 arg2) {
    SpuVmSeKeyOff(0x21, arg0, arg1, arg2);
}
