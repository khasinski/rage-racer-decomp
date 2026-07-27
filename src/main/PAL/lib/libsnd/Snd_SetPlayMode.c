#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void func_80076C58(long seq_sep, u_short left, u_short right, long mode);

void func_800724F0(long seq, long sep, long playMode, long loopCount) {
    short sepIndex;
    long sequenceBase;
    long sepOffset;
    long mode;
    long seqOffset;
    SeqStruct **sequence;
    SeqStruct *state;
    SeqStruct *flagsState;
    SeqStruct *playState;
    SeqStruct *stopState;

    seqOffset = seq << 16;
    sequenceBase = (long)D_801E79CC;
    sequence = (SeqStruct **)((seqOffset >> 14) + sequenceBase);
    sepIndex = sep;
    sepOffset = sepIndex * 0xAC;
    seqOffset = sepOffset;
    state = (SeqStruct *)(sepOffset + (long)*sequence);
    state->flags &= ~0x200;
    flagsState = (SeqStruct *)(sepOffset + (long)*sequence);
    mode = playMode & 0xFF;
    flagsState->flags &= ~4;
    state->unk46 = loopCount;

    if (mode == 1) {
        playState = (SeqStruct *)(seqOffset + (long)*sequence);
        playState->flags |= 1;
        state->unk48 = 0;
        state->unk2b = 1;
        func_80076C58((short)(seq | (sep << 8)), state->unk74, state->unk76, 0);
    } else if (mode == 0) {
        stopState = (SeqStruct *)(seqOffset + (long)*sequence);
        stopState->flags |= 2;
    }
}

void func_800725F0(long seq, long playMode, long loopCount) {
    func_800724F0((short)seq, 0, playMode & 0xFF, (short)loopCount);
}

void func_80072628(long seq, long sep, long playMode, long loopCount) {
    func_800724F0((short)seq, (short)sep, playMode & 0xFF, (short)loopCount);
}

void func_80072660(long seq, long sep, long left, long right) {
    func_80076C58((short)(seq | (sep << 8)), (u_short)left, (u_short)right, 0);
}

void func_80072698(long seq, long left, long right) {
    func_80076C58((short)seq, (u_short)left, (u_short)right, 0);
}

void func_800726C8(long seq, long sep, long left, long right) {
    func_80076C58((short)(seq | (sep << 8)), (u_short)left, (u_short)right, 0);
}

void func_80076DCC(long seqSep, long value, long ticks);

void func_80072700(long seq, long sep, long value, long ticks) {
    func_80076DCC((short)(seq | (sep << 8)), value, ticks);
}

void func_80076ED8(long seqSep);

void func_80072734(short seq, short sep) {
    long i;
    SeqStruct *score;

    score = &D_801E79CC[seq][sep];
    D_801E79CC[seq][sep].flags &= ~1;
    D_801E79CC[seq][sep].flags &= ~2;
    D_801E79CC[seq][sep].flags &= ~8;
    D_801E79CC[seq][sep].flags |= 4;
    func_80076ED8((sep << 8) | seq);

    score->delta_value = score->base_delta_value;
    score->tempo = score->base_unk84;
    score->tick_period = score->unk72;
    score->read_pos = score->next_sep_pos;
    score->loop_pos = score->next_sep_pos;
    score->unk2b = 0;
    score->unk80 = 0;
    score->unk27 = 0;
    score->unk13 = 0;
    score->play_mode = 0;
    score->unk29 = 0;
    score->unk15 = 0;
    score->unk16 = 0;
    score->unk2a = 0;
    score->channel = 0;
    score->unk48 = 0;
    score->unk27 = 0;
    score->unk28 = 0;
    score->unk10 = 0;
    score->unk11 = 0;

    for (i = 0; i < 16; i++) {
        score->programs[i] = i;
        score->panpot[i] = 0x40;
        score->vol[i] = 0x7F;
    }

    score->unk78 = 0x7F;
    score->unk7A = 0x7F;
}

void func_800728A0(long seq) {
    func_80072734((short)seq, 0);
}

void func_800728C8(long seq, long sep) {
    func_80072734((short)seq, (short)sep);
}
