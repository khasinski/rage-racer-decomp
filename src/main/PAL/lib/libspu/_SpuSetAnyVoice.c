#include <sys/types.h>

#include "common.h"
#include "psyq/spu.h"

extern volatile u_short *g_SpuRegBase;

u_long _SpuSetAnyVoice(long arg0, u_long arg1, long arg2, long arg3) {
    volatile u_short *reg_hi;
    volatile u_short *reg_lo;
    register u_long hi asm("$2");
    u_long lo;
    u_long old;

    {
        volatile u_short *base = g_SpuRegBase;

        reg_hi = (volatile u_short *)((u_long)(arg3 << 1) + (u_long)base);
        reg_lo = (volatile u_short *)((u_long)(arg2 << 1) + (u_long)base);
    }

    hi = *reg_hi;
    lo = *reg_lo;
    hi &= 0xFF;
    hi <<= 0x10;
    old = lo | hi;

    if (arg0 != 0) {
        if (arg0 != 1) {
            return old & 0xFFFFFF;
        }

        old |= arg1 & 0xFFFFFF;
        *reg_lo |= arg1;
        *reg_hi |= (arg1 >> 0x10) & 0xFF;
    } else {
        old &= ~(arg1 & 0xFFFFFF);
        *reg_lo &= ~arg1;
        *reg_hi &= ~((arg1 >> 0x10) & 0xFF);
    }

    return old & 0xFFFFFF;
}
