#include "psyq/spu.h"

extern volatile u_short *D_8009AB7C;
extern volatile u_long *D_8009AB80;
extern volatile u_long *D_8009AB84;
extern volatile u_long *D_8009AB88;
extern volatile u_long *D_8009AB90;
extern long D_8009ABC8;

void _spu_startDmaTransfer(u_long arg0, u_short arg1, u_long arg2) {
    u_long addr = arg0;
    volatile long i;
    volatile long delay;
    u_short cnt;
    u_long value;

    D_8009AB7C[0xD3] = arg1;

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    cnt = D_8009AB7C[0xD5];
    D_8009AB7C[0xD5] = cnt | 0x30;

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    value = *D_8009AB90;
    value &= 0xF0FFFFFF;
    value |= 0x22000000;
    *D_8009AB90 = value;

    *D_8009AB80 = addr;
    *D_8009AB84 = (arg2 << 0x10) | 0x10;
    D_8009ABC8 = 1;
    *D_8009AB88 = 0x01000200;
}
