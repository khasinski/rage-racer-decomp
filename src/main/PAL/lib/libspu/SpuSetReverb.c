#include "common.h"
#include "psyq/spu.h"

extern volatile long D_8009A718;
extern long D_8009A71C;
extern long D_8009A720;
extern SpuRxx *D_8009AB7C;

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
