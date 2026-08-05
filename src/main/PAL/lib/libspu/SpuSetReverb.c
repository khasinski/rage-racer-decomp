#include "common.h"
#include "psyq/spu.h"

extern volatile long D_8009A718;
extern long g_SpuRevReserveWa;
extern long g_SpuRevWorkAreaAddr;
extern SpuCommonRegs *g_SpuRegBase;

long SpuSetReverb(long on_off) {
    register u_short cnt asm("v1");

    if (on_off != 0) {
        if (on_off == 1) {
        } else {
        return D_8009A718;
        }
    } else {

    cnt = g_SpuRegBase->spuCnt;
    D_8009A718 = 0;
    g_SpuRegBase->spuCnt = cnt & 0xFF7F;
    return D_8009A718;
    }

    if (g_SpuRevReserveWa != on_off && _SpuIsInAllocateArea_(g_SpuRevWorkAreaAddr) != 0) {
        cnt = g_SpuRegBase->spuCnt;
        D_8009A718 = 0;
        g_SpuRegBase->spuCnt = cnt & 0xFF7F;
        return D_8009A718;
    }

    cnt = g_SpuRegBase->spuCnt;
    D_8009A718 = on_off;
    g_SpuRegBase->spuCnt = cnt | 0x80;

    return D_8009A718;
}
