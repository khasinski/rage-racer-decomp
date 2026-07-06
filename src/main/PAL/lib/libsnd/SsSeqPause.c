#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void SsSeqPause(s32 seq, s32 sep) {
    register s32 offset asm("s0");
    register SeqStruct *state asm("s1");
    register SeqStruct **seq_entry asm("s2");
    register SeqStruct **seq_table asm("v0");

    seq = (s16)seq;
    seq_table = D_801E79CC;
    seq_entry = &seq_table[seq];
    sep = (s16)sep;
    offset = sep * sizeof(SeqStruct);
    state = (SeqStruct *)((u8 *)*seq_entry + offset);
    SpuVmSeqKeyOff(((s16)sep << 8) | (s16)seq);
    state->unk2b = 0;
    offset = (s32)((u8 *)*seq_entry + offset);
    ((SeqStruct *)offset)->flags &= ~SS_SEQ_FLAG_PAUSED;
}
