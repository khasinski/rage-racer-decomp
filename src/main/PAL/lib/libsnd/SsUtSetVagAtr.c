#include "common.h"
#include "psyq/snd.h"

extern volatile u_char *g_SndCurrentToneTable;

long SsUtSetVagAtr(long arg0, long arg1, long arg2, VagAtr *arg3) {
    long index;
    long offset;

    index = (short)arg0;
    if (g_SndVabStatus[index] == 1) {
        SpuVmVSetUp(index, (short)arg1);
        offset = (short)(arg2 + (g_SndCurrentProgActual << 4)) << 5;

        /* Preserves the original `offset + base` addu operand order. */
        *(volatile u_char *)(offset + (long)g_SndCurrentToneTable) = arg3->prior;
        g_SndCurrentToneTable[offset + 0x1] = arg3->mode;
        g_SndCurrentToneTable[offset + 0x2] = arg3->vol;
        g_SndCurrentToneTable[offset + 0x3] = arg3->pan;
        g_SndCurrentToneTable[offset + 0x4] = arg3->center;
        g_SndCurrentToneTable[offset + 0x5] = arg3->shift;
        g_SndCurrentToneTable[offset + 0x7] = arg3->max;
        g_SndCurrentToneTable[offset + 0x6] = arg3->min;
        g_SndCurrentToneTable[offset + 0x8] = arg3->vibW;
        g_SndCurrentToneTable[offset + 0x9] = arg3->vibT;
        g_SndCurrentToneTable[offset + 0xA] = arg3->porW;
        g_SndCurrentToneTable[offset + 0xB] = arg3->porT;
        g_SndCurrentToneTable[offset + 0xC] = arg3->pbmin;
        g_SndCurrentToneTable[offset + 0xD] = arg3->pbmax;
        *(u_short *)&g_SndCurrentToneTable[offset + 0x10] = arg3->adsr1;
        *(u_short *)&g_SndCurrentToneTable[offset + 0x12] = arg3->adsr2;
        *(u_short *)&g_SndCurrentToneTable[offset + 0x14] = arg3->prog;
        *(u_short *)&g_SndCurrentToneTable[offset + 0x16] = arg3->vag;

        return 0;
    }
    return -1;
}
