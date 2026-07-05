#include "common.h"

extern s32 D_8009B2D0;
extern s32 D_801E40D4;

void func_80052158(s32 arg0, s32 arg1, s32 arg2);

s32 func_800562C8(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2D0 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2D0;
        D_8009B2D0 = value;
        if (value >= 0x1FD) {
            D_8009B2D0 = 0x1FC;
        }
        value = 0;
    } else {
        register s32 limit asm("v1");
        register u32 product asm("v0");

        value = arg0 + D_8009B2D0;
        D_8009B2D0 = value;
        limit = 0x1FC;
        if (value < 0) {
            D_8009B2D0 = 0;
        }
        limit = limit - D_8009B2D0;
        product = (u32)(limit * limit);
        value = product >> 11;
    }

    func_80052158((s16)value, ((u32)D_8009B2D0 >> 2) & 0xFF, D_801E40D4);
    return D_8009B2D0;
}
