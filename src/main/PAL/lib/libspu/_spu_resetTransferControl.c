#include <sys/types.h>

#include "common.h"

extern volatile u_short *g_SpuRegBase;

long _spu_resetTransferControl(void) {
    volatile long i;
    volatile long delay;
    u_short cnt;

    cnt = g_SpuRegBase[0xD5];
    g_SpuRegBase[0xD5] = cnt & 0x7FCF;

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    {
        volatile u_short *base = g_SpuRegBase;

        cnt &= 0xFFCF;
        base[0xD5] = cnt;
    }
    return 0;
}
