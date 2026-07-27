#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");
extern long g_SndSeqOpenMask asm("D_801E4B80");
extern short g_SndSeqTableTMax asm("D_801E8270");

void func_80076C58(long seq_sep, u_short left, u_short right, long arg3);

void SsSeqClose(long seq) {
    volatile long pad[2];
    long seq_s;
    volatile SeqStruct **slot;
    volatile SeqStruct **table;
    long seq_offset;
    long offset;
    long index;
    long value;
    /* This pin is load-bearing: removing it changes .text. */
    register long current asm("$3");
    long mask;

    seq <<= 16;
    seq_s = seq >> 16;
    func_80076C58(seq_s, 0, 0, 1);
    SpuVmSeqKeyOff(seq_s);

    mask = ~(1 << seq_s);
    g_SndSeqOpenMask = g_SndSeqOpenMask & mask;

    index = 0;
    if (g_SndSeqTableTMax > 0) {
        table = (volatile SeqStruct **)g_SndSeqTable;
        seq_offset = seq_s * 4;
        slot = (volatile SeqStruct **)(seq_offset + (long)table);
        value = 0x7F;
        offset = 0;
        do {
            ((SeqStruct *)(offset + (long)*slot))->flags = 0;
            current = 0xFF;
            ((SeqStruct *)(offset + (long)*slot))->unk3C = current;
            ((SeqStruct *)(offset + (long)*slot))->unk0 = 0;
            ((SeqStruct *)(offset + (long)*slot))->unk3E = 0;
            ((SeqStruct *)(offset + (long)*slot))->unk40 = 0;
            ((SeqStruct *)(offset + (long)*slot))->unk94 = 0;
            ((SeqStruct *)(offset + (long)*slot))->unk98 = 0;
            ((SeqStruct *)(offset + (long)*slot))->unk42 = 0;
            ((SeqStruct *)(offset + (long)*slot))->target_tempo = 0;
            ((SeqStruct *)(offset + (long)*slot))->tempo_countdown = 0;
            ((SeqStruct *)(offset + (long)*slot))->unk9C = 0;
            ((SeqStruct *)(offset + (long)*slot))->tempo_step = 0;
            index++;
            ((SeqStruct *)(offset + (long)*slot))->unk74 = value;
            current = offset + (long)*slot;
            offset += sizeof(SeqStruct);
            ((SeqStruct *)current)->unk76 = value;
        } while (index < g_SndSeqTableTMax);
    }
}
