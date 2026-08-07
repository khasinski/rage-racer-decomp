#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];

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
            g_SndSeqTable[i][j].unk3C = 0xFF;
            g_SndSeqTable[i][j].unk0 = 0;
            g_SndSeqTable[i][j].unk3E = 0;
            g_SndSeqTable[i][j].unk40 = 0;
            g_SndSeqTable[i][j].unk94 = 0;
            g_SndSeqTable[i][j].unk98 = 0;
            g_SndSeqTable[i][j].unk42 = 0;
            g_SndSeqTable[i][j].target_tempo = 0;
            g_SndSeqTable[i][j].tempo_countdown = 0;
            g_SndSeqTable[i][j].unk9C = 0;
            g_SndSeqTable[i][j].tempo_step = 0;
            g_SndSeqTable[i][j].left_volume = 0x7F;
            g_SndSeqTable[i][j].right_volume = 0x7F;
            g_SndSeqTable[i][j].unk78 = 0x7F;
            g_SndSeqTable[i][j].unk7A = 0x7F;
        }
    }
}
