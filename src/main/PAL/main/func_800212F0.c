#include "common.h"

extern u8 * volatile D_8009E67C;

void func_800212F0(s32 arg0) {
    u8 *ptr = D_8009E67C;

    *(s16 *)(ptr + 6) = 5;
    ptr[3] = 0;
    ptr[2] = 0;
    ptr[1] = 0;
    ptr[0] = 0;

    if (arg0 < 2) {
        D_8009E67C[3] = 0xFF;
    }

    *(s16 *)(D_8009E67C + 4) = 0;
}
