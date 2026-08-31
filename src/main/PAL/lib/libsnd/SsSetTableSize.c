#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

#include "psyq/snd_internal.h"

void SsSetTableSize(u_char *table, short seq_count, short sep_count) {
    long i;
    long j;
    SeqStruct *scores = (SeqStruct *)table;

    g_SndSeqTableSMax = seq_count;
    g_SndSeqTableTMax = sep_count;
    for (i = 0; i < seq_count; i++) {
        g_SndSeqTable[i] = &scores[i * sep_count];
    }
    for (i = 0; i < g_SndSeqTableSMax; i++) {
        for (j = 0; j < g_SndSeqTableTMax; j++) {
            g_SndSeqTable[i][j].flags = 0;
            g_SndSeqTable[i][j].restart_seq = 0xFF;
            g_SndSeqTable[i][j].restart_sep = 0;
            g_SndSeqTable[i][j].fade_volume_range = 0;
            g_SndSeqTable[i][j].fade_steps_left = 0;
            g_SndSeqTable[i][j].fade_ticks_total = 0;
            g_SndSeqTable[i][j].fade_ticks_left = 0;
            g_SndSeqTable[i][j].fade_step = 0;
            g_SndSeqTable[i][j].target_tempo = 0;
            g_SndSeqTable[i][j].tempo_countdown = 0;
            g_SndSeqTable[i][j].reserved9C = 0;
            g_SndSeqTable[i][j].tempo_step = 0;
            g_SndSeqTable[i][j].left_volume = 0x7F;
            g_SndSeqTable[i][j].right_volume = 0x7F;
            g_SndSeqTable[i][j].cur_vol_left = 0x7F;
            g_SndSeqTable[i][j].cur_vol_right = 0x7F;
        }
    }
}
