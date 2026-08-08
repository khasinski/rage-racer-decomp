#include <sys/types.h>

#include "common.h"
#include "psyq/spu.h"
#include "psyq/spu_internal.h"


u_long _SpuSetAnyVoice(long mode, u_long voiceBits, long regLow, long regHigh) {
    volatile u_short *reg_hi;
    volatile u_short *reg_lo;
    register u_long hi asm("$2");
    u_long lo;
    u_long old;

    {
        volatile u_short *base = g_SpuRegBase;

        reg_hi = (volatile u_short *)((u_long)(regHigh << 1) + (u_long)base);
        reg_lo = (volatile u_short *)((u_long)(regLow << 1) + (u_long)base);
    }

    hi = *reg_hi;
    lo = *reg_lo;
    hi &= 0xFF;
    hi <<= 0x10;
    old = lo | hi;

    if (mode != 0) {
        if (mode != 1) {
            return old & 0xFFFFFF;
        }

        old |= voiceBits & 0xFFFFFF;
        *reg_lo |= voiceBits;
        *reg_hi |= (voiceBits >> 0x10) & 0xFF;
    } else {
        old &= ~(voiceBits & 0xFFFFFF);
        *reg_lo &= ~voiceBits;
        *reg_hi &= ~((voiceBits >> 0x10) & 0xFF);
    }

    return old & 0xFFFFFF;
}
