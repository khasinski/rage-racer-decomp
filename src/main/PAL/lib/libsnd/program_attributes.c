#include "common.h"
#include "psyq/snd.h"

#include "psyq/snd_internal.h"

long SsUtGetProgAtr(long vab_id, long program, ProgAtr *out) {
    long chan = (short)vab_id;
    long index;
    long offset;

    if (g_SndVabStatus[chan] == 1) {
        index = (short)program;
        SpuVmVSetUp(chan, index);
        offset = index * 0x10;

        out->tones = *(u_char *)(offset + (long)g_SndCurrentProgTable);
        out->mvol = ((u_char *)g_SndCurrentProgTable)[offset + 1];
        out->prior = ((u_char *)g_SndCurrentProgTable)[offset + 2];
        out->mode = ((u_char *)g_SndCurrentProgTable)[offset + 3];
        out->mpan = ((u_char *)g_SndCurrentProgTable)[offset + 4];
        offset = (long)((u_char *)g_SndCurrentProgTable + offset);
        out->attr = *(u_short *)(offset + 6);
    } else {
        return -1;
    }

    return 0;
}

long SpuVmVSetUp(long vab_id, long program) {
    register long raw0 asm("$6") = vab_id;
    register long raw1 asm("$7") = program;
    long chan;
    long index;
    u_char *data;
    u_char *entry;

    switch (0) { default:
    if ((u_short)raw0 < 0x10) {
        chan = (short)vab_id;
        if (g_SndVabStatus[chan] != 1) {
            return -1;
        }
        index = (short)program;
        if (index < g_SndVabProgMax) {
            break;
        }
    }

    return -1;

    }
    {
        u_char *meta = (u_char *)g_SndVabHeader[chan];
        register u_char *base asm("$4") = (u_char *)g_SndVabProgTable[chan];

        data = (u_char *)g_SndVabToneTable[chan];
        g_SndCurrentVabId = raw0;
        g_SndCurrentProg = raw1;
        g_SndCurrentToneTable = (VagAtr *)data;
        entry = (u_char *)((index << 4) + (long)base);
        g_SndCurrentVabHeader = (VabHdr *)meta;
        g_SndCurrentProgTable = (ProgAtr *)base;
        g_SndCurrentProgActual = entry[8];
    }
    return 0;
}

long SsUtGetVagAtr(long vab_id, long program, long tone, VagAtr *out) {
    long chan;
    long offset;

    chan = (short)vab_id;
    if (g_SndVabStatus[chan] == 1) {
        SpuVmVSetUp(chan, (short)program);

        offset = (long)(short)(tone + (g_SndCurrentProgActual << 4)) << 5;

        out->prior = *(u_char *)(offset + (long)g_SndCurrentToneTable);
        out->mode = ((u_char *)g_SndCurrentToneTable)[offset + 1];
        out->vol = ((u_char *)g_SndCurrentToneTable)[offset + 2];
        out->pan = ((u_char *)g_SndCurrentToneTable)[offset + 3];
        out->center = ((u_char *)g_SndCurrentToneTable)[offset + 4];
        out->shift = ((u_char *)g_SndCurrentToneTable)[offset + 5];
        out->max = ((u_char *)g_SndCurrentToneTable)[offset + 7];
        out->min = ((u_char *)g_SndCurrentToneTable)[offset + 6];
        out->vibW = ((u_char *)g_SndCurrentToneTable)[offset + 8];
        out->vibT = ((u_char *)g_SndCurrentToneTable)[offset + 9];
        out->porW = ((u_char *)g_SndCurrentToneTable)[offset + 10];
        out->porT = ((u_char *)g_SndCurrentToneTable)[offset + 11];
        out->pbmin = ((u_char *)g_SndCurrentToneTable)[offset + 12];
        out->pbmax = ((u_char *)g_SndCurrentToneTable)[offset + 13];

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
