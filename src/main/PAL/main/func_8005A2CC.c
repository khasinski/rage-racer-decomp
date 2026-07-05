#include "common.h"

extern u32 D_8009B2EC;
extern s32 D_801E40D4;

void func_80052158(s32 arg0, s32 arg1, s32 arg2);

u32 func_8005A2CC(s32 arg0) {
    u32 value;
    s32 amount;

    if (arg0 == 0) {
        D_8009B2EC = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2EC;
        D_8009B2EC = value;
        if ((s32)value >= 0x1FD) {
            D_8009B2EC = 0x1FC;
        }
        amount = 0;
    } else {
        register s32 diff asm("$3") = 0x1FC;
        register s32 product asm("$2");

        value = arg0 + D_8009B2EC;
        D_8009B2EC = value;
        if ((s32)value < 0) {
            D_8009B2EC = 0;
        }
        diff -= D_8009B2EC;
        product = diff * diff;
        amount = (u32)product >> 11;
    }

    func_80052158((s16)amount, (D_8009B2EC >> 2) & 0xFF, D_801E40D4);
    return D_8009B2EC;
}
