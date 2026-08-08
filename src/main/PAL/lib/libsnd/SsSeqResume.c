#include "psyq/snd.h"

#include "psyq/snd_internal.h"

void SsSeqResume(long seq, long sep) {
    long raw;
    SeqStruct **table;
    SeqStruct **slot;
    long offset;
    SeqStruct *base;
    long one;
    long index;

    raw = seq << 16;
    table = g_SndSeqTable;
    slot = (SeqStruct **)((raw >> 14) + (long)table);
    index = (short)sep;
    offset = index * 0xAC;
    base = *slot;
    one = 1;
    ((SeqStruct *)(offset + (long)base))->unk2b = one;
    base = *slot;
    offset += (long)base;
    ((SeqStruct *)offset)->flags = ((SeqStruct *)offset)->flags & -9;
}
