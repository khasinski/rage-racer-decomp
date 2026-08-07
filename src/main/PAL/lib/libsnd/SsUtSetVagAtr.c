#include "common.h"
#include "psyq/snd.h"

extern volatile u_char *g_SndCurrentToneTable;

long SsUtSetVagAtr(long vab_id, long program, long tone, VagAtr *in) {
    long index;
    long offset;

    index = (short)vab_id;
    if (g_SndVabStatus[index] == 1) {
        SpuVmVSetUp(index, (short)program);
        offset = (short)(tone + (g_SndCurrentProgActual << 4)) << 5;

        /* Preserves the original `offset + base` addu operand order. */
        *(volatile u_char *)(offset + (long)g_SndCurrentToneTable) = in->prior;
        g_SndCurrentToneTable[offset + 0x1] = in->mode;
        g_SndCurrentToneTable[offset + 0x2] = in->vol;
        g_SndCurrentToneTable[offset + 0x3] = in->pan;
        g_SndCurrentToneTable[offset + 0x4] = in->center;
        g_SndCurrentToneTable[offset + 0x5] = in->shift;
        g_SndCurrentToneTable[offset + 0x7] = in->max;
        g_SndCurrentToneTable[offset + 0x6] = in->min;
        g_SndCurrentToneTable[offset + 0x8] = in->vibW;
        g_SndCurrentToneTable[offset + 0x9] = in->vibT;
        g_SndCurrentToneTable[offset + 0xA] = in->porW;
        g_SndCurrentToneTable[offset + 0xB] = in->porT;
        g_SndCurrentToneTable[offset + 0xC] = in->pbmin;
        g_SndCurrentToneTable[offset + 0xD] = in->pbmax;
        *(u_short *)&g_SndCurrentToneTable[offset + 0x10] = in->adsr1;
        *(u_short *)&g_SndCurrentToneTable[offset + 0x12] = in->adsr2;
        *(u_short *)&g_SndCurrentToneTable[offset + 0x14] = in->prog;
        *(u_short *)&g_SndCurrentToneTable[offset + 0x16] = in->vag;

        return 0;
    }
    return -1;
}
