#include "psyq/spu.h"

extern volatile u_long D_8009A76C;
extern volatile u_short *D_8009AB7C;

long SpuGetKeyStatus(u_long arg0) {
    long voice = -1;
    long i = 0;
    u_long mask = 1;
    register u_long value asm("$3");
    register long ret asm("$2");

loop:
    if (arg0 & (mask << i)) {
        goto found;
    }
    i++;
    if (i < 24) {
        goto loop;
    }

postscan:
    if (voice != -1) {
        goto body;
    }
    goto ret_minus_one;

found:
    voice = i;
    goto postscan;

body:
    {
        register u_long offset asm("$3") = voice << 4;
        register volatile u_short *base asm("$2") = D_8009AB7C;
        register u_long flags asm("$4") = D_8009A76C;
        register u_long flag asm("$2");

        offset += (u_long)base;
        flag = (1 << voice) & flags;
        asm("" : "=r"(flag) : "0"(flag));
        value = *(volatile u_short *)(offset + 0xC);
        if (flag != 0) {
            ret = 3;
            if (value == 0) {
                goto ret_value;
            }
            ret = 1;
            goto ret_value;
        }

        ret = value != 0 ? 2 : 0;
        goto ret_value;
    }

ret_minus_one:
    return -1;

ret_value:
    return ret;
}
