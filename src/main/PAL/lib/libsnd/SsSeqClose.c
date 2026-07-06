#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];
extern s32 D_801E4B80;
extern s16 D_801E8270;

void func_80076C58(s32 seq_sep, u16 left, u16 right, s32 arg3);

void SsSeqClose(s32 seq) {
    volatile s32 pad[2];
    register s32 seq_s asm("$16");
    register volatile SeqStruct **slot asm("$5");
    register volatile SeqStruct **table asm("$3");
    register s32 seq_offset asm("$2");
    register s32 offset asm("$4");
    register s32 index asm("$6");
    register s32 value asm("$7");
    register s32 current asm("$3");
    s32 mask;

    seq <<= 16;
    asm("sra %0,%1,16" : "=r"(seq_s) : "r"(seq));
    func_80076C58(seq_s, 0, 0, 1);
    SpuVmSeqKeyOff(seq_s);

    mask = ~(1 << seq_s);
    D_801E4B80 = D_801E4B80 & mask;

    index = 0;
    if (D_801E8270 > 0) {
        table = (volatile SeqStruct **)D_801E79CC;
        seq_offset = seq_s * 4;
        slot = (volatile SeqStruct **)(seq_offset + (s32)table);
        value = 0x7F;
        offset = 0;
        do {
            ((SeqStruct *)(offset + (s32)*slot))->flags = 0;
            current = 0xFF;
            ((SeqStruct *)(offset + (s32)*slot))->unk3C = current;
            ((SeqStruct *)(offset + (s32)*slot))->unk0 = 0;
            ((SeqStruct *)(offset + (s32)*slot))->unk3E = 0;
            ((SeqStruct *)(offset + (s32)*slot))->unk40 = 0;
            ((SeqStruct *)(offset + (s32)*slot))->unk94 = 0;
            ((SeqStruct *)(offset + (s32)*slot))->unk98 = 0;
            ((SeqStruct *)(offset + (s32)*slot))->unk42 = 0;
            ((SeqStruct *)(offset + (s32)*slot))->target_tempo = 0;
            ((SeqStruct *)(offset + (s32)*slot))->tempo_countdown = 0;
            ((SeqStruct *)(offset + (s32)*slot))->unk9C = 0;
            ((SeqStruct *)(offset + (s32)*slot))->tempo_step = 0;
            index++;
            ((SeqStruct *)(offset + (s32)*slot))->unk74 = value;
            current = offset + (s32)*slot;
            offset += sizeof(SeqStruct);
            ((SeqStruct *)current)->unk76 = value;
        } while (index < D_801E8270);
    }
}
