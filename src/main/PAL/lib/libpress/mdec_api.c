#include <sys/types.h>

#include "common.h"
#include "psyq/kernel.h"
#include "psyq/cd.h"
#include "psyq/press_internal.h"

void DecDCTReset(long mode) {
    if (mode == 0) {
        KernelCallbackSlot3();
    }
    MDEC_reset(mode);
}

u_long *DecDCTGetEnv(u_long *env) {
    u_long *dst;
    u_long *src;
    long i;

    dst = env;
    src = g_MdecQuantLuma;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *src++;
    }

    dst = env + 0x10;
    src = g_MdecQuantChroma;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *src++;
    }

    dst = env + 0x20;
    src = g_MdecIdctTable;
    for (i = 0x1F; i != -1; i--) {
        *dst++ = *src++;
    }

    return env;
}

u_long *DecDCTPutEnv(u_long *env) {
    u_long *ret;
    u_long *dst;
    long i;

    ret = env;
    dst = g_MdecQuantLuma;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *env++;
    }

    dst = g_MdecQuantChroma;
    env = ret + 0x10;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *env++;
    }

    MDEC_in((volatile u_long *)g_MdecQuantCmd, 0x20);
    MDEC_in((volatile u_long *)g_MdecIdctCmd, 0x20);

    return ret;
}

long DecDCTBufSize(u_short *bitstream) {
    return *bitstream;
}

void DecDCTin(volatile u_long *bitstream, long mode) {
    u_long mask;

    if (mode % 2) {
        bitstream[0] &= 0xF7FFFFFF;
    } else {
        bitstream[0] |= 0x08000000;
    }

    mask = 0x02000000;
    if (mode & 2) {
        bitstream[0] |= mask;
    } else {
        bitstream[0] &= 0xFDFFFFFF;
    }

    MDEC_in(bitstream, *(u_short *)bitstream);
}
