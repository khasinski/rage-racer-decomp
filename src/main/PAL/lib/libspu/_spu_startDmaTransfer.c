#include "psyq/spu.h"

extern volatile u16 *D_8009AB7C;
extern volatile u32 *D_8009AB80;
extern volatile u32 *D_8009AB84;
extern volatile u32 *D_8009AB88;
extern volatile u32 *D_8009AB90;
extern s32 D_8009ABC8;

void _spu_startDmaTransfer(u32 arg0, u16 arg1, u32 arg2) {
    register u32 addr asm("$7") = arg0;
    volatile s32 i;
    volatile s32 delay;
    u16 cnt;
    u32 value;

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
