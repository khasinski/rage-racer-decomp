#include "common.h"

extern u8 D_801E4CFC[];
extern s16 D_801E40D0;
extern u8 D_801E4BD1;
extern u8 D_801E4BD6;
extern u8 D_801E4BD7;
extern u8 *D_801E416C;
extern volatile u8 *D_801E413C;
extern volatile u8 *D_801E4110;
extern u8 *D_8019CA20[];
extern u8 *D_8019C9B0[];
extern u8 *D_8019CA70[];

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");

s32 SpuVmVSetUp(s32 arg0, s32 arg1) {
    register s32 raw0 asm("$6") = arg0;
    register s32 raw1 asm("$7") = arg1;
    s32 chan;
    s32 index;
    u8 *data;
    u8 *entry;

    if ((u16)raw0 < 0x10) {
        chan = (s16)arg0;
        if (D_801E4CFC[chan] != 1) {
            return -1;
        }
        index = (s16)arg1;
        if (index < D_801E40D0) {
            goto success;
        }
    }

fail:
    return -1;

success:
    {
        register u8 *meta asm("$3") = D_8019CA20[chan];
        register u8 *base asm("$4") = D_8019C9B0[chan];

        data = D_8019CA70[chan];
        D_801E4BD1 = raw0;
        D_801E4BD6 = raw1;
        D_801E416C = data;
        entry = (u8 *)((index << 4) + (s32)base);
        D_801E413C = meta;
        D_801E4110 = base;
        D_801E4BD7 = entry[8];
    }
    return 0;
}
