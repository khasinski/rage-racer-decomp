#include "common.h"

extern volatile u16 *D_8009AB7C;

s32 _spu_resetTransferControl(void) {
    volatile s32 i;
    volatile s32 delay;
    u16 cnt;

    cnt = D_8009AB7C[0xD5];
    D_8009AB7C[0xD5] = cnt & 0x7FCF;

    delay = 0xD;
    i = 0;
    while (i < 0xF0) {
        delay *= 3;
        i++;
    }

    {
        volatile u16 *base = D_8009AB7C;

        cnt &= 0xFFCF;
        base[0xD5] = cnt;
    }
    return 0;
}
