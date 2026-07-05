#include "common.h"

void SsStartSoundTick(s32 arg0) asm("func_80071C44");

void SsStartSoundTickMode0(void) asm("func_80071ECC");

void SsStartSoundTickMode0(void) {
    SsStartSoundTick(0);
}
