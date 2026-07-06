#include "common.h"
#include "psyq/spu.h"


INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libsnd/SsUtSetReverbType", SsUtSetReverbType);

extern SpuReverbAttr D_8019C778;
extern s16 D_8019C77C;

s32 SsUtGetReverbType(void) asm("func_800736B8");
s32 SsUtGetReverbType(void) {
    return D_8019C77C;
}

void SsUtReverbOn(void) asm("func_800736C8");
void SsUtReverbOn(void) {
    SpuSetReverb(1);
}

void SsUtReverbOff(void) asm("func_800736E8");
void SsUtReverbOff(void) {
    SpuSetReverb(0);
}

asm(".globl func_800736F8\nfunc_800736F8 = func_800736E8 + 0x10");

void SsUtSetReverbFeedback(s32 feedback) asm("func_80073708");
void SsUtSetReverbFeedback(s32 feedback) {
    D_8019C778.mask = 0x10;
    D_8019C778.feedback = (s16)feedback;
    SpuSetReverbModeParam(&D_8019C778);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libsnd/SsUtSetReverbType", SsUtSetReverbDepth);
INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libsnd/SsUtSetReverbType", SsUtSetReverbDelay);
