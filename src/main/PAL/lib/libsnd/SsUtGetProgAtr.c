#include "common.h"
#include "psyq/snd.h"

extern u_char g_SndVabStatus[] asm("D_801E4CFC");
extern short g_SndVabProgMax asm("D_801E40D0");
extern u_char g_SndCurrentVabId asm("D_801E4BD1");
extern u_char g_SndCurrentProg asm("D_801E4BD6");
extern u_char g_SndCurrentProgActual asm("D_801E4BD7");
extern u_char *g_SndCurrentToneTable asm("D_801E416C");
extern volatile u_char *g_SndCurrentVabHeader asm("D_801E413C");
extern u_char *g_SndCurrentProgTable asm("D_801E4110");
extern u_char *g_SndVabHeader[] asm("D_8019CA20");
extern u_char *g_SndVabProgTable[] asm("D_8019C9B0");
extern u_char *g_SndVabToneTable[] asm("D_8019CA70");

long SpuVmVSetUp(long arg0, long arg1) asm("func_80073314");
long SsUtGetVagAtr(long arg0, long arg1, long arg2, VagAtr *out) asm("func_800733D8");

long SsUtGetProgAtr(long arg0, long arg1, ProgAtr *out) asm("func_8007320C");

long SsUtGetProgAtr(long arg0, long arg1, ProgAtr *out) {
    long chan = (short)arg0;
    long index;
    long offset;

    if (g_SndVabStatus[chan] == 1) {
        index = (short)arg1;
        SpuVmVSetUp(chan, index);
        offset = index * 0x10;

        out->tones = *(u_char *)(offset + (long)g_SndCurrentProgTable);
        out->mvol = g_SndCurrentProgTable[offset + 1];
        out->prior = g_SndCurrentProgTable[offset + 2];
        out->mode = g_SndCurrentProgTable[offset + 3];
        out->mpan = g_SndCurrentProgTable[offset + 4];
        offset = (long)(g_SndCurrentProgTable + offset);
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
        if (g_SndVabStatus[chan] != 1) {
            return -1;
        }
        index = (short)arg1;
        if (index < g_SndVabProgMax) {
            goto success;
        }
    }

fail:
    return -1;

success:
    {
        register u_char *meta asm("$3") = g_SndVabHeader[chan];
        register u_char *base asm("$4") = g_SndVabProgTable[chan];

        data = g_SndVabToneTable[chan];
        g_SndCurrentVabId = raw0;
        g_SndCurrentProg = raw1;
        g_SndCurrentToneTable = data;
        entry = (u_char *)((index << 4) + (long)base);
        g_SndCurrentVabHeader = meta;
        g_SndCurrentProgTable = base;
        g_SndCurrentProgActual = entry[8];
    }
    return 0;
}

long SsUtGetVagAtr(long arg0, long arg1, long arg2, VagAtr *out) {
    long chan;
    register long offset asm("$3");

    chan = (short)arg0;
    if (g_SndVabStatus[chan] == 1) {
        SpuVmVSetUp(chan, (short)arg1);

        offset = (long)(short)(arg2 + (g_SndCurrentProgActual << 4)) << 5;

        out->prior = *(u_char *)(offset + (long)g_SndCurrentToneTable);
        out->mode = g_SndCurrentToneTable[offset + 1];
        out->vol = g_SndCurrentToneTable[offset + 2];
        out->pan = g_SndCurrentToneTable[offset + 3];
        out->center = g_SndCurrentToneTable[offset + 4];
        out->shift = g_SndCurrentToneTable[offset + 5];
        out->max = g_SndCurrentToneTable[offset + 7];
        out->min = g_SndCurrentToneTable[offset + 6];
        out->vibW = g_SndCurrentToneTable[offset + 8];
        out->vibT = g_SndCurrentToneTable[offset + 9];
        out->porW = g_SndCurrentToneTable[offset + 10];
        out->porT = g_SndCurrentToneTable[offset + 11];
        out->pbmin = g_SndCurrentToneTable[offset + 12];
        out->pbmax = g_SndCurrentToneTable[offset + 13];

        offset = offset + (long)g_SndCurrentToneTable;
        out->adsr1 = *(u_short *)(offset + 0x10);
        out->adsr2 = *(u_short *)(offset + 0x12);
        out->prog = *(u_short *)(offset + 0x14);
        out->vag = *(u_short *)(offset + 0x16);

        return 0;
    } else {
        return -1;
    }
}
