#include "common.h"
#include "psyq/spu.h"

extern SpuReverbAttr D_8019C778;
extern short D_8019C77C;

long SsUtSetReverbType(long type) asm("func_80073614");
long SsUtSetReverbType(long type) {
    register long normalized asm("v1");
    register long negative asm("a1");
    register long mode asm("v0");
    register long result asm("s0");

    negative = 0;
    normalized = type;
    if ((long)(type << 16) < 0) {
        negative = 1;
        normalized = -type;
    }

    if ((u_long)(normalized & 0xFFFF) >= 10) {
        goto fail;
    }

    D_8019C778.mask = 1;
    if (negative != 0) {
        mode = (normalized | 0x100) << 16;
    } else {
        mode = normalized << 16;
    }
    mode >>= 16;
    *(long *)&D_8019C77C = mode;
    mode = normalized << 16;
    result = mode >> 16;

    if (result == 0) {
        asm volatile(
            ".globl func_800736F8\n"
            "func_800736F8 = func_800736E8 + 0x10\n"
            ".word 0x0C01E8B4\n"
            ".word 0x00002021");
    }
    SpuSetReverbModeParam(&D_8019C778);
    return result;

fail:
    return -1;
}

long SsUtGetReverbType(void) asm("func_800736B8");
long SsUtGetReverbType(void) {
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


void SsUtSetReverbFeedback(long feedback) asm("func_80073708");
void SsUtSetReverbFeedback(long feedback) {
    D_8019C778.mask = 0x10;
    D_8019C778.feedback = (short)feedback;
    SpuSetReverbModeParam(&D_8019C778);
}

void SsUtSetReverbDepth(long left, long right) asm("func_80073748");
void SsUtSetReverbDepth(long left, long right) {
    D_8019C778.mask = 0x6;
    D_8019C778.depth.left = ((short)left * 0x7FFF) / 0x7F;
    D_8019C778.depth.right = ((short)right * 0x7FFF) / 0x7F;
    SpuSetReverbModeParam(&D_8019C778);
}

void SsUtSetReverbDelay(long delay) asm("func_800737E0");
void SsUtSetReverbDelay(long delay) {
    D_8019C778.mask = 0x8;
    D_8019C778.delay = (short)delay;
    SpuSetReverbModeParam(&D_8019C778);
}
