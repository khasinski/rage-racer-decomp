#include "common.h"
#include "psyq/snd.h"

extern short g_SndVabProgMax;
extern u_char *g_SndCurrentToneTable;
extern volatile u_char *g_SndCurrentVabHeader;
extern u_char *g_SndCurrentProgTable;
extern u_char *g_SndVabHeader[];
extern u_char *g_SndVabProgTable[];
extern u_char *g_SndVabToneTable[];

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

    switch (0) { default:
    if ((u_short)raw0 < 0x10) {
        chan = (short)arg0;
        if (g_SndVabStatus[chan] != 1) {
            return -1;
        }
        index = (short)arg1;
        if (index < g_SndVabProgMax) {
            break;
        }
    }

    return -1;

    }
    {
        u_char *meta = g_SndVabHeader[chan];
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
    long offset;

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
