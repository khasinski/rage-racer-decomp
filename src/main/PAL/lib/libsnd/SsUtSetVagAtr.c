#include "common.h"
#include "psyq/snd.h"

#include "psyq/snd_internal.h"

long SsUtSetVagAtr(long vab_id, long program, long tone, VagAtr *in) {
    long index;
    long offset;

    index = (short)vab_id;
    if (g_SndVabStatus[index] == 1) {
        SpuVmVSetUp(index, (short)program);
        offset = (short)(tone + (g_SndCurrentProgActual << 4)) << 5;

        /* Preserves the original `offset + base` addu operand order. */
        *(volatile u_char *)(offset + (long)g_SndCurrentToneTable) = in->prior;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x1] = in->mode;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x2] = in->vol;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x3] = in->pan;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x4] = in->center;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x5] = in->shift;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x7] = in->max;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x6] = in->min;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x8] = in->vibW;
        ((u_char *)g_SndCurrentToneTable)[offset + 0x9] = in->vibT;
        ((u_char *)g_SndCurrentToneTable)[offset + 0xA] = in->porW;
        ((u_char *)g_SndCurrentToneTable)[offset + 0xB] = in->porT;
        ((u_char *)g_SndCurrentToneTable)[offset + 0xC] = in->pbmin;
        ((u_char *)g_SndCurrentToneTable)[offset + 0xD] = in->pbmax;
        *(u_short *)&((u_char *)g_SndCurrentToneTable)[offset + 0x10] = in->adsr1;
        *(u_short *)&((u_char *)g_SndCurrentToneTable)[offset + 0x12] = in->adsr2;
        *(u_short *)&((u_char *)g_SndCurrentToneTable)[offset + 0x14] = in->prog;
        *(u_short *)&((u_char *)g_SndCurrentToneTable)[offset + 0x16] = in->vag;

        return 0;
    }
    return -1;
}
