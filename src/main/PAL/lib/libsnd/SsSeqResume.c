#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

void SsSeqResume(long arg0, long arg1) asm("func_800718E0");

void SsSeqResume(long arg0, long arg1) {
    long raw;
    SeqStruct **table;
    SeqStruct **slot;
    long offset;
    SeqStruct *base;
    long one;
    long index;

    raw = arg0 << 16;
    table = g_SndSeqTable;
    slot = (SeqStruct **)((raw >> 14) + (long)table);
    index = (short)arg1;
    offset = index * 0xAC;
    base = *slot;
    one = 1;
    ((SeqStruct *)(offset + (long)base))->unk2b = one;
    base = *slot;
    offset += (long)base;
    ((SeqStruct *)offset)->flags = ((SeqStruct *)offset)->flags & -9;
}
