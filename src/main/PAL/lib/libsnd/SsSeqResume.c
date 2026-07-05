#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void SsSeqResume(s32 arg0, s32 arg1) asm("func_800718E0");

void SsSeqResume(s32 arg0, s32 arg1) {
    register s32 raw asm("$4");
    register SeqStruct **table asm("$2");
    register SeqStruct **slot asm("$4");
    register s32 offset asm("$2");
    register SeqStruct *base asm("$3");
    register s32 one asm("$5");
    s32 index;

    raw = arg0 << 16;
    table = D_801E79CC;
    slot = (SeqStruct **)((raw >> 14) + (s32)table);
    asm("" : "=r"(slot) : "0"(slot));
    index = (s16)arg1;
    offset = index * 0xAC;
    base = *slot;
    one = 1;
    ((SeqStruct *)(offset + (s32)base))->unk2b = one;
    base = *slot;
    offset += (s32)base;
    ((SeqStruct *)offset)->flags = ((SeqStruct *)offset)->flags & -9;
}
