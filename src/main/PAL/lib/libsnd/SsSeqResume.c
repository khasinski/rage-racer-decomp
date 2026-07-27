#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void SsSeqResume(long arg0, long arg1) asm("func_800718E0");

void SsSeqResume(long arg0, long arg1) {
    register long raw asm("$4");
    register SeqStruct **table asm("$2");
    register SeqStruct **slot asm("$4");
    register long offset asm("$2");
    register SeqStruct *base asm("$3");
    register long one asm("$5");
    long index;

    raw = arg0 << 16;
    table = D_801E79CC;
    slot = (SeqStruct **)((raw >> 14) + (long)table);
    asm("" : "=r"(slot) : "0"(slot));
    index = (short)arg1;
    offset = index * 0xAC;
    base = *slot;
    one = 1;
    ((SeqStruct *)(offset + (long)base))->unk2b = one;
    base = *slot;
    offset += (long)base;
    ((SeqStruct *)offset)->flags = ((SeqStruct *)offset)->flags & -9;
}
