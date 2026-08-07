#include "psyq/spu.h"

extern volatile u_long g_SpuKeyStatus;
extern volatile u_short *g_SpuRegBase;

long SpuGetKeyStatus(u_long voice_bit) {
    long voice = -1;
    long i = 0;
    u_long mask = 1;
    u_short value;
    long ret;

do {
    if (voice_bit & (mask << i)) {
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
        value = *(volatile u_short *)(offset + 0xC);
        if (flag != 0) {
            ret = 3;
            if (value != 0) {
                ret = 1;
            }
            return ret;
        }

        return value != 0 ? 2 : 0;
    }

    return -1;

}
