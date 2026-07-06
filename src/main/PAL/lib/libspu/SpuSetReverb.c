#include "common.h"
#include "psyq/spu.h"

extern volatile s32 D_8009A718;
extern s32 D_8009A71C;
extern s32 D_8009A720;
extern SpuRxx *D_8009AB7C;

s32 SpuSetReverb(s32 on_off) {
    register s32 value asm("s0") = on_off;
    register u16 cnt asm("v1");

    if (value != 0) {
        if (value == 1) {
            goto check_on;
        }
        goto done;
    }

disable:
    cnt = D_8009AB7C->spucnt;
    asm volatile(
        "lui $1,%hi(D_8009A718)\n"
        "sw $0,%lo(D_8009A718)($1)");
    D_8009AB7C->spucnt = cnt & 0xFF7F;
    goto done;

check_on:
    if (D_8009A71C != value && _SpuIsInAllocateArea_(D_8009A720) != 0) {
        cnt = D_8009AB7C->spucnt;
        D_8009A718 = 0;
        D_8009AB7C->spucnt = cnt & 0xFF7F;
        goto done;
    }

    cnt = D_8009AB7C->spucnt;
    D_8009A718 = value;
    D_8009AB7C->spucnt = cnt | 0x80;

done:
    return D_8009A718;
}
