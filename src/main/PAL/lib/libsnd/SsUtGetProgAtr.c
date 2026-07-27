#include "common.h"
#include "psyq/snd.h"

extern u_char D_801E4CFC[];
extern short D_801E40D0;
extern u_char D_801E4BD1;
extern u_char D_801E4BD6;
extern u_char D_801E4BD7;
extern u_char *D_801E416C;
extern volatile u_char *D_801E413C;
extern u_char *D_801E4110;
extern u_char *D_8019CA20[];
extern u_char *D_8019C9B0[];
extern u_char *D_8019CA70[];

long SpuVmVSetUp(long arg0, long arg1) asm("func_80073314");
long SsUtGetVagAtr(long arg0, long arg1, long arg2, VagAtr *out) asm("func_800733D8");

long SsUtGetProgAtr(long arg0, long arg1, ProgAtr *out) asm("func_8007320C");

long SsUtGetProgAtr(long arg0, long arg1, ProgAtr *out) {
    long chan = (short)arg0;
    long index;
    long offset;

    if (D_801E4CFC[chan] == 1) {
        index = (short)arg1;
        SpuVmVSetUp(chan, index);
        offset = index * 0x10;

        out->tones = *(u_char *)(offset + (long)D_801E4110);
        out->mvol = D_801E4110[offset + 1];
        out->prior = D_801E4110[offset + 2];
        out->mode = D_801E4110[offset + 3];
        out->mpan = D_801E4110[offset + 4];
        offset = (long)(D_801E4110 + offset);
        out->attr = *(u_short *)(offset + 6);
    } else {
        return -1;
    }

    return 0;
}

long SpuVmVSetUp(long arg0, long arg1) {
    register long raw0 asm("$6") = arg0;
    register long raw1 asm("$7") = arg1;
    long chan;
    long index;
    u_char *data;
    u_char *entry;

    if ((u_short)raw0 < 0x10) {
        chan = (short)arg0;
        if (D_801E4CFC[chan] != 1) {
            return -1;
        }
        index = (short)arg1;
        if (index < D_801E40D0) {
            goto success;
        }
    }

fail:
    return -1;

success:
    {
        register u_char *meta asm("$3") = D_8019CA20[chan];
        register u_char *base asm("$4") = D_8019C9B0[chan];

        data = D_8019CA70[chan];
        D_801E4BD1 = raw0;
        D_801E4BD6 = raw1;
        D_801E416C = data;
        entry = (u_char *)((index << 4) + (long)base);
        D_801E413C = meta;
        D_801E4110 = base;
        D_801E4BD7 = entry[8];
    }
    return 0;
}

long SsUtGetVagAtr(long arg0, long arg1, long arg2, VagAtr *out) {
    long chan;
    register long offset asm("$3");

    chan = (short)arg0;
    if (D_801E4CFC[chan] == 1) {
        SpuVmVSetUp(chan, (short)arg1);

        offset = (long)(short)(arg2 + (D_801E4BD7 << 4)) << 5;

        out->prior = *(u_char *)(offset + (long)D_801E416C);
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

        offset = offset + (long)D_801E416C;
        out->adsr1 = *(u_short *)(offset + 0x10);
        out->adsr2 = *(u_short *)(offset + 0x12);
        out->prog = *(u_short *)(offset + 0x14);
        out->vag = *(u_short *)(offset + 0x16);

        return 0;
    } else {
        return -1;
    }
}
