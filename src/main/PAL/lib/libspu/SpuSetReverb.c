#include "common.h"
#include "psyq/spu.h"

extern volatile long D_8009A718;
extern long g_SpuRevReserveWa asm("D_8009A71C");
extern long g_SpuRevWorkAreaAddr asm("D_8009A720");
extern SpuRxx *g_SpuRegBase asm("D_8009AB7C");

long SpuSetReverb(long on_off) {
    /* This pin is load-bearing: removing it changes .text. */
    register u_short cnt asm("v1");

    if (on_off != 0) {
        if (on_off == 1) {
            goto check_on;
        }
        goto done;
    }

disable:
    cnt = g_SpuRegBase->spucnt;
    D_8009A718 = 0;
    g_SpuRegBase->spucnt = cnt & 0xFF7F;
    goto done;

check_on:
    if (g_SpuRevReserveWa != on_off && _SpuIsInAllocateArea_(g_SpuRevWorkAreaAddr) != 0) {
        cnt = g_SpuRegBase->spucnt;
        D_8009A718 = 0;
        g_SpuRegBase->spucnt = cnt & 0xFF7F;
        goto done;
    }

    cnt = g_SpuRegBase->spucnt;
    D_8009A718 = on_off;
    g_SpuRegBase->spucnt = cnt | 0x80;

done:
    return D_8009A718;
}
