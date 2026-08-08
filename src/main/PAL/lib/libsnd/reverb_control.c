#include "common.h"
#include "psyq/spu.h"
#include "psyq/snd_internal.h"

short SsUtSetReverbType(short type) {
    short normalized;
    long negative;

    negative = 0;
    normalized = type;
    if (type < 0) {
        negative = 1;
        normalized = -type;
    }

    if (normalized >= 0 && normalized < 10) {
        g_SndReverbAttr.mask = 1;
        if (negative != 0) {
            g_SndReverbAttr.mode = normalized | 0x100;
        } else {
            g_SndReverbAttr.mode = normalized;
        }
        if (normalized == 0) {
            SpuSetReverb(0);
        }
        SpuSetReverbModeParam(&g_SndReverbAttr);
        return normalized;
    }
    return -1;
}

short SsUtGetReverbType(void) {
    return g_SndReverbAttr.mode;
}

void SsUtReverbOn(void) {
    SpuSetReverb(1);
}

void SsUtReverbOff(void) {
    SpuSetReverb(0);
}


void SsUtSetReverbFeedback(long feedback) {
    g_SndReverbAttr.mask = 0x10;
    g_SndReverbAttr.feedback = (short)feedback;
    SpuSetReverbModeParam(&g_SndReverbAttr);
}

void SsUtSetReverbDepth(long left, long right) {
    g_SndReverbAttr.mask = 0x6;
    g_SndReverbAttr.depth.left = ((short)left * 0x7FFF) / 0x7F;
    g_SndReverbAttr.depth.right = ((short)right * 0x7FFF) / 0x7F;
    SpuSetReverbModeParam(&g_SndReverbAttr);
}

void SsUtSetReverbDelay(long delay) {
    g_SndReverbAttr.mask = 0x8;
    g_SndReverbAttr.delay = (short)delay;
    SpuSetReverbModeParam(&g_SndReverbAttr);
}
