#include "common.h"
#include "psyq/spu.h"

extern SpuReverbAttr g_SndReverbAttr asm("D_8019C778");
extern short g_SndReverbAttrMode asm("D_8019C77C");

long SsUtSetReverbType(long type) asm("func_80073614");
long SsUtSetReverbType(long type) {
    /* This pin is load-bearing: removing it changes .text. */
    register long normalized asm("v1");
    long negative;
    long mode;
    long result;

    negative = 0;
    normalized = type;
    if ((long)(type << 16) < 0) {
        negative = 1;
        normalized = -type;
    }

    if ((u_long)(normalized & 0xFFFF) >= 10) {
        goto fail;
    }

    g_SndReverbAttr.mask = 1;
    if (negative != 0) {
        mode = (normalized | 0x100) << 16;
    } else {
        mode = normalized << 16;
    }
    mode >>= 16;
    *(long *)&g_SndReverbAttrMode = mode;
    mode = normalized << 16;
    result = mode >> 16;

    if (result == 0) {
        SpuSetReverb(0);
    }
    SpuSetReverbModeParam(&g_SndReverbAttr);
    return result;

fail:
    return -1;
}

long SsUtGetReverbType(void) asm("func_800736B8");
long SsUtGetReverbType(void) {
    return g_SndReverbAttrMode;
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
    g_SndReverbAttr.mask = 0x10;
    g_SndReverbAttr.feedback = (short)feedback;
    SpuSetReverbModeParam(&g_SndReverbAttr);
}

void SsUtSetReverbDepth(long left, long right) asm("func_80073748");
void SsUtSetReverbDepth(long left, long right) {
    g_SndReverbAttr.mask = 0x6;
    g_SndReverbAttr.depth.left = ((short)left * 0x7FFF) / 0x7F;
    g_SndReverbAttr.depth.right = ((short)right * 0x7FFF) / 0x7F;
    SpuSetReverbModeParam(&g_SndReverbAttr);
}

void SsUtSetReverbDelay(long delay) asm("func_800737E0");
void SsUtSetReverbDelay(long delay) {
    g_SndReverbAttr.mask = 0x8;
    g_SndReverbAttr.delay = (short)delay;
    SpuSetReverbModeParam(&g_SndReverbAttr);
}
