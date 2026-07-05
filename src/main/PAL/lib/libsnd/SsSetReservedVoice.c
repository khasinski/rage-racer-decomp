#include "common.h"

extern u8 D_801E4D88;

void SsSetReservedVoice(u8 arg0) asm("func_80072AD0");

void SsSetReservedVoice(u8 arg0) {
    D_801E4D88 = arg0;
}
