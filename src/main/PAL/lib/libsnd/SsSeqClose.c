#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];

void SpuVmSetSeqVol(long seq_sep, u_short left, u_short right, long mode);

void SsSeqClose(short seq) {
    long i;

    SpuVmSetSeqVol(seq, 0, 0, 1);
    SpuVmSeqKeyOff(seq);
    g_SndSeqOpenMask &= ~(1 << seq);
    for (i = 0; i < g_SndSeqTableTMax; i++) {
        g_SndSeqTable[seq][i].flags = 0;
        g_SndSeqTable[seq][i].unk3C = 0xFF;
        g_SndSeqTable[seq][i].unk0 = 0;
        g_SndSeqTable[seq][i].unk3E = 0;
        g_SndSeqTable[seq][i].unk40 = 0;
        g_SndSeqTable[seq][i].unk94 = 0;
        g_SndSeqTable[seq][i].unk98 = 0;
        g_SndSeqTable[seq][i].unk42 = 0;
        g_SndSeqTable[seq][i].target_tempo = 0;
        g_SndSeqTable[seq][i].tempo_countdown = 0;
        g_SndSeqTable[seq][i].unk9C = 0;
        g_SndSeqTable[seq][i].tempo_step = 0;
        g_SndSeqTable[seq][i].left_volume = 0x7F;
        g_SndSeqTable[seq][i].right_volume = 0x7F;
    }
}
