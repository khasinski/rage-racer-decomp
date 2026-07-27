#include "common.h"
#include "psyq/spu.h"

extern volatile long D_8009A718;
extern long g_SpuRevReserveWa asm("D_8009A71C");
extern long g_SpuRevWorkAreaAddr asm("D_8009A720");
extern SpuRxx *g_SpuRegBase asm("D_8009AB7C");

long SpuSetReverb(long on_off) {
    register long value asm("s0") = on_off;
    register u_short cnt asm("v1");

    if (value != 0) {
        if (value == 1) {
            goto check_on;
        }
        goto done;
    }

disable:
    cnt = g_SpuRegBase->spucnt;
    asm volatile(
        "lui $1,%hi(D_8009A718)\n"
        "sw $0,%lo(D_8009A718)($1)");
    g_SpuRegBase->spucnt = cnt & 0xFF7F;
    goto done;

check_on:
    if (g_SpuRevReserveWa != value && _SpuIsInAllocateArea_(g_SpuRevWorkAreaAddr) != 0) {
        cnt = g_SpuRegBase->spucnt;
        D_8009A718 = 0;
        g_SpuRegBase->spucnt = cnt & 0xFF7F;
        goto done;
    }

    cnt = g_SpuRegBase->spucnt;
    D_8009A718 = value;
    g_SpuRegBase->spucnt = cnt | 0x80;

done:
    return D_8009A718;
}
