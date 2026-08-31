#include "psyq/snd.h"

#include "psyq/snd_internal.h"

void SsSeqClose(short seq) {
    long i;

    SpuVmSetSeqVol(seq, 0, 0, 1);
    SpuVmSeqKeyOff(seq);
    g_SndSeqOpenMask &= ~(1 << seq);
    for (i = 0; i < g_SndSeqTableTMax; i++) {
        g_SndSeqTable[seq][i].flags = 0;
        g_SndSeqTable[seq][i].restart_seq = 0xFF;
        g_SndSeqTable[seq][i].restart_sep = 0;
        g_SndSeqTable[seq][i].fade_volume_range = 0;
        g_SndSeqTable[seq][i].fade_steps_left = 0;
        g_SndSeqTable[seq][i].fade_ticks_total = 0;
        g_SndSeqTable[seq][i].fade_ticks_left = 0;
        g_SndSeqTable[seq][i].fade_step = 0;
        g_SndSeqTable[seq][i].target_tempo = 0;
        g_SndSeqTable[seq][i].tempo_countdown = 0;
        g_SndSeqTable[seq][i].reserved9C = 0;
        g_SndSeqTable[seq][i].tempo_step = 0;
        g_SndSeqTable[seq][i].left_volume = 0x7F;
        g_SndSeqTable[seq][i].right_volume = 0x7F;
    }
}
