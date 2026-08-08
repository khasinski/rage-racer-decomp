#include "common.h"
#include "psyq/spu.h"
#include "psyq/spu_internal.h"


long SpuSetReverb(long on_off) {
    short cnt;

    switch (on_off) {
    case 0:
        g_SpuRevState = 0;
        cnt = g_SpuRegBase->regs.spuCnt;
        cnt &= ~0x80;
        g_SpuRegBase->regs.spuCnt = cnt;
        break;
    case 1:
        if (g_SpuRevReserveWa != on_off && _SpuIsInAllocateArea_(g_SpuRevWorkAreaAddr)) {
            g_SpuRevState = 0;
            cnt = g_SpuRegBase->regs.spuCnt;
            cnt &= ~0x80;
            g_SpuRegBase->regs.spuCnt = cnt;
        } else {
            g_SpuRevState = on_off;
            cnt = g_SpuRegBase->regs.spuCnt;
            cnt |= 0x80;
            g_SpuRegBase->regs.spuCnt = cnt;
        }
        break;
    }

    return g_SpuRevState;
}
