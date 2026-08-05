#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];

void SsSeqPause(long seq, long sep) {
    long offset;
    SeqStruct *state;
    SeqStruct **seq_entry;
    SeqStruct **seq_table;

    seq = (short)seq;
    seq_table = g_SndSeqTable;
    seq_entry = &seq_table[seq];
    sep = (short)sep;
    offset = sep * sizeof(SeqStruct);
    state = (SeqStruct *)((u_char *)*seq_entry + offset);
    SpuVmSeqKeyOff(((short)sep << 8) | (short)seq);
    state->unk2b = 0;
    offset = (long)((u_char *)*seq_entry + offset);
    ((SeqStruct *)offset)->flags &= ~SS_SEQ_FLAG_PAUSED;
}
