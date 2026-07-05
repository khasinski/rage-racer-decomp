#include "common.h"
#include "psyq/snd.h"

extern u8 D_801E4CFC[];
extern s16 D_801E40D0;
extern u8 D_801E4BD1;
extern u8 D_801E4BD6;
extern u8 D_801E4BD7;
extern u8 *D_801E416C;
extern volatile u8 *D_801E413C;
extern u8 *D_801E4110;
extern u8 *D_8019CA20[];
extern u8 *D_8019C9B0[];
extern u8 *D_8019CA70[];

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");
s32 SsUtGetVagAtr(s32 arg0, s32 arg1, s32 arg2, VagAtr *out) asm("func_800733D8");

s32 SsUtGetProgAtr(s32 arg0, s32 arg1, ProgAtr *out) asm("func_8007320C");

s32 SsUtGetProgAtr(s32 arg0, s32 arg1, ProgAtr *out) {
    s32 chan = (s16)arg0;
    s32 index;
    s32 offset;

    if (D_801E4CFC[chan] == 1) {
        index = (s16)arg1;
        SpuVmVSetUp(chan, index);
        offset = index * 0x10;

        out->tones = *(u8 *)(offset + (s32)D_801E4110);
        out->mvol = D_801E4110[offset + 1];
        out->prior = D_801E4110[offset + 2];
        out->mode = D_801E4110[offset + 3];
        out->mpan = D_801E4110[offset + 4];
        offset = (s32)(D_801E4110 + offset);
        out->attr = *(u16 *)(offset + 6);
    } else {
        return -1;
    }

    return 0;
}

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
