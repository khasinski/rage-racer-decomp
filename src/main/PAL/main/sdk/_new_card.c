#include <sys/types.h>

#include "common.h"
#include "psyq/kernel.h"
#include "psyq/cd.h"

u_long *DecDCTGetEnv(u_long *arg0);
u_long *DecDCTPutEnv(u_long *arg0);

u_long _new_card[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090050,
    0,
};

void MDEC_reset(long arg0);

void DecDCTReset(long arg0) {
    if (arg0 == 0) {
        KernelCallbackSlot3();
    }
    MDEC_reset(arg0);
}

/* The two MDEC parameter blocks, each a command word followed by its table:
 * g_MdecQuantCmd 0x40000001 (MDEC command 2, set quant tables, colour) with
 * the 64-byte luma and chroma tables, and g_MdecIdctCmd 0x60000000
 * (command 3, set scale table) with the 64-halfword IDCT cosine table. */
extern u_long g_MdecQuantLuma[];
extern u_long g_MdecQuantChroma[];
extern u_long g_MdecIdctTable[];

u_long * DecDCTGetEnv(u_long *arg0);
u_long *DecDCTGetEnv(u_long *arg0) {
    u_long *dst;
    u_long *src;
    long i;

    dst = arg0;
    src = g_MdecQuantLuma;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *src++;
    }

    dst = arg0 + 0x10;
    src = g_MdecQuantChroma;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *src++;
    }

    dst = arg0 + 0x20;
    src = g_MdecIdctTable;
    for (i = 0x1F; i != -1; i--) {
        *dst++ = *src++;
    }

    return arg0;
}

u_long * DecDCTPutEnv(u_long *arg0);
u_long *DecDCTPutEnv(u_long *arg0) {
    u_long *ret;
    u_long *dst;
    long i;

    ret = arg0;
    dst = g_MdecQuantLuma;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *arg0++;
    }

    dst = g_MdecQuantChroma;
    arg0 = ret + 0x10;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *arg0++;
    }

    MDEC_in((volatile u_long *)g_MdecQuantCmd, 0x20);
    MDEC_in((volatile u_long *)g_MdecIdctCmd, 0x20);

    return ret;
}

long DecDCTBufSize(u_short *arg0) {
    return *arg0;
}

void DecDCTin(volatile u_long *arg0, long arg1) {
    u_long mask;

    if (arg1 % 2) {
        arg0[0] &= 0xF7FFFFFF;
    } else {
        arg0[0] |= 0x08000000;
    }

    mask = 0x02000000;
    if (arg1 & 2) {
        arg0[0] |= mask;
    } else {
        arg0[0] &= 0xFDFFFFFF;
    }

    MDEC_in(arg0, *(u_short *)arg0);
}
