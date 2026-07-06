#include "common.h"

extern s32 D_8009B2E8;
extern s32 D_801E40D4;

void func_80052158(s32 arg0, s32 arg1, s32 arg2);

s32 func_80059248(s32 arg0) {
    register s32 value asm("v0");
    register s32 limit asm("v1");
    register s32 amount asm("a0");
    register s32 phase asm("a1");
    register s32 channel asm("a2");

    if (arg0 == 0) {
        D_8009B2E8 = 0;
        return;
    }

    if (arg0 > 0) {
        value = D_8009B2E8 + arg0;
        D_8009B2E8 = value;
        if (value >= 0x1FD) {
            D_8009B2E8 = 0x1FC;
        }
        value = 0;
        goto update;
    }

    value = D_8009B2E8 + arg0;
    D_8009B2E8 = value;
    if (value < 0) {
        D_8009B2E8 = 0;
    }

    value = D_8009B2E8;
    limit = 0x1FC;
    limit -= value;
    value = (u32)(limit * limit) >> 0xB;

update:
    asm("sll %0,%1,0x10\n"
        "sra %0,%0,0x10"
        : "=r"(amount)
        : "r"(value));
    phase = ((u32)D_8009B2E8 >> 2) & 0xFF;
    channel = D_801E40D4;
    func_80052158(amount, phase, channel);

    return D_8009B2E8;
}
