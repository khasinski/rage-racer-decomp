#include "common.h"

void SsStartSoundTick(s32 arg0) asm("func_80071C44");

void SsStartSoundTickMode1(void) asm("func_80071EAC");

void SsStartSoundTickMode1(void) {
    SsStartSoundTick(1);
}
