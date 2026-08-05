#include "psyq/spu.h"

extern volatile u_long g_SpuKeyStatus;
extern volatile u_short *g_SpuRegBase;

long SpuGetKeyStatus(u_long arg0) {
    long voice = -1;
    long i = 0;
    u_long mask = 1;
    register u_long value asm("$3");
    register long ret asm("$2");

do {
    if (arg0 & (mask << i)) {
        goto found;
    }
    i++;
} while (i < 24);

for (;;) {
    if (!(voice != -1)) {
    return -1;

found:
    voice = i;
    continue;

    }
break;
}
    {
        u_long offset = voice << 4;
        volatile u_short *base = g_SpuRegBase;
        u_long flags = g_SpuKeyStatus;
        u_long flag;

        offset += (u_long)base;
        flag = (1 << voice) & flags;
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

    return -1;

    return ret;
}
