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

void SsUtSetReverbDepth(s32 left, s32 right) asm("func_80073748");
void SsUtSetReverbDepth(s32 left, s32 right) {
    D_8019C778.mask = 0x6;
    D_8019C778.depth.left = ((s16)left * 0x7FFF) / 0x7F;
    D_8019C778.depth.right = ((s16)right * 0x7FFF) / 0x7F;
    SpuSetReverbModeParam(&D_8019C778);
}

void SsUtSetReverbDelay(s32 delay) asm("func_800737E0");
void SsUtSetReverbDelay(s32 delay) {
    D_8019C778.mask = 0x8;
    D_8019C778.delay = (s16)delay;
    SpuSetReverbModeParam(&D_8019C778);
}
