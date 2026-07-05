#include "common.h"
#include "psyq/snd.h"

extern u8 D_801E4CFC[];
extern u8 D_801E4BD7;
extern u8 *D_801E416C;

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");

s32 SsUtGetVagAtr(s32 arg0, s32 arg1, s32 arg2, VagAtr *out) asm("func_800733D8");

s32 SsUtGetVagAtr(s32 arg0, s32 arg1, s32 arg2, VagAtr *out) {
    s32 chan;
    register s32 offset asm("$3");

    chan = (s16)arg0;
    if (D_801E4CFC[chan] == 1) {
        SpuVmVSetUp(chan, (s16)arg1);

        offset = (s32)(s16)(arg2 + (D_801E4BD7 << 4)) << 5;

        out->prior = *(u8 *)(offset + (s32)D_801E416C);
        out->mode = D_801E416C[offset + 1];
        out->vol = D_801E416C[offset + 2];
        out->pan = D_801E416C[offset + 3];
        out->center = D_801E416C[offset + 4];
        out->shift = D_801E416C[offset + 5];
        out->max = D_801E416C[offset + 7];
        out->min = D_801E416C[offset + 6];
        out->vibW = D_801E416C[offset + 8];
        out->vibT = D_801E416C[offset + 9];
        out->porW = D_801E416C[offset + 10];
        out->porT = D_801E416C[offset + 11];
        out->pbmin = D_801E416C[offset + 12];
        out->pbmax = D_801E416C[offset + 13];

        offset = offset + (s32)D_801E416C;
        out->adsr1 = *(u16 *)(offset + 0x10);
        out->adsr2 = *(u16 *)(offset + 0x12);
        out->prog = *(u16 *)(offset + 0x14);
        out->vag = *(u16 *)(offset + 0x16);

        return 0;
    } else {
        return -1;
    }
}
