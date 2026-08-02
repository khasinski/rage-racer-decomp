#include "psyq/spu.h"

extern volatile u_long g_SpuKeyStatus asm("D_8009A76C");
extern volatile u_short *g_SpuRegBase asm("D_8009AB7C");

long SpuGetKeyStatus(u_long arg0) {
    long voice = -1;
    long i = 0;
    u_long mask = 1;
    /* These pins are load-bearing: removing any one changes .text. */
    register u_long value asm("$3");
    register long ret asm("$2");

do {
    if (arg0 & (mask << i)) {
        goto found;
    }
    i++;
} while (i < 24);

postscan:
    if (!(voice != -1)) {
    return -1;

found:
    voice = i;
    goto postscan;

    }
    {
        u_long offset = voice << 4;
        volatile u_short *base = g_SpuRegBase;
        u_long flags = g_SpuKeyStatus;
        u_long flag;

        offset += (u_long)base;
        flag = (1 << voice) & flags;
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" : "=r"(flag) : "0"(flag));
        value = *(volatile u_short *)(offset + 0xC);
        if (flag != 0) {
            ret = 3;
            if (value == 0) {
                return ret;
            }
            ret = 1;
            return ret;
        }

        ret = value != 0 ? 2 : 0;
        return ret;
    }

ret_minus_one:
    return -1;

ret_value:
    return ret;
}
