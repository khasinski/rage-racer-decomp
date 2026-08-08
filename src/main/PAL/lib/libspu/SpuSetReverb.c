#include "common.h"
#include "psyq/spu.h"

extern volatile long g_SpuRevState;
extern SpuCommonRegs *g_SpuRegBase;

long SpuSetReverb(long on_off) {
    register u_short cnt asm("v1");

    if (on_off != 0) {
        if (on_off == 1) {
        } else {
        return g_SpuRevState;
        }
    } else {

    cnt = g_SpuRegBase->spuCnt;
    g_SpuRevState = 0;
    g_SpuRegBase->spuCnt = cnt & 0xFF7F;
    return g_SpuRevState;
    }

    if (g_SpuRevReserveWa != on_off && _SpuIsInAllocateArea_(g_SpuRevWorkAreaAddr) != 0) {
        cnt = g_SpuRegBase->spuCnt;
        g_SpuRevState = 0;
        g_SpuRegBase->spuCnt = cnt & 0xFF7F;
        return g_SpuRevState;
    }

    cnt = g_SpuRegBase->spuCnt;
    g_SpuRevState = on_off;
    g_SpuRegBase->spuCnt = cnt | 0x80;

    return g_SpuRevState;
}
