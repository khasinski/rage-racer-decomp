#include "common.h"
#include "psyq/spu.h"

extern SpuReverbAttr D_8019C778;
extern s16 D_8019C77C;

s32 SsUtSetReverbType(s32 type) asm("func_80073614");
s32 SsUtSetReverbType(s32 type) {
    register s32 normalized asm("v1");
    register s32 negative asm("a1");
    register s32 mode asm("v0");
    register s32 result asm("s0");

    negative = 0;
    normalized = type;
    if ((s32)(type << 16) < 0) {
        negative = 1;
        normalized = -type;
    }

    if ((u32)(normalized & 0xFFFF) >= 10) {
        goto fail;
    }

    D_8019C778.mask = 1;
    if (negative != 0) {
        mode = (normalized | 0x100) << 16;
    } else {
        mode = normalized << 16;
    }
    mode >>= 16;
    *(s32 *)&D_8019C77C = mode;
    mode = normalized << 16;
    result = mode >> 16;

    if (result == 0) {
        asm volatile(
            ".word 0x0C01E8B4\n"
            ".word 0x00002021");
    }
    SpuSetReverbModeParam(&D_8019C778);
    return result;

fail:
    return -1;
}

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
