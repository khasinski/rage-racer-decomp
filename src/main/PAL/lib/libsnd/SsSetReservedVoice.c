#include "common.h"

extern u8 D_801E4D88;
extern s16 D_801E3FB0;

void SsSetReservedVoice(u8 arg0) asm("func_80072AD0");

void SsSetReservedVoice(u8 arg0) {
    D_801E4D88 = arg0;
}

void SsSetMono(void) asm("func_80072AE0");

void SsSetMono(void) {
    D_801E3FB0 = 1;
}

void SsSetStereo(void) asm("func_80072AF4");

void SsSetStereo(void) {
    D_801E3FB0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/PAL/main", SsSetVoiceCount);
