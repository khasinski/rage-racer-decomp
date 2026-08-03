#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

void func_80076C58(long seq_sep, u_short left, u_short right, long mode);

void Snd_SetPlayMode(long seq, long sep, long playMode, long loopCount) asm("func_800724F0");
void Snd_SetPlayMode(long seq, long sep, long playMode, long loopCount) {
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
    sequenceBase = (long)g_SndSeqTable;
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
        func_80076C58((short)(seq | (sep << 8)), state->left_volume, state->right_volume, 0);
    } else if (mode == 0) {
        stopState = (SeqStruct *)(seqOffset + (long)*sequence);
        stopState->flags |= 2;
    }
}

void SsSeqPlay(long seq, long playMode, long loopCount) asm("func_800725F0");
void SsSeqPlay(long seq, long playMode, long loopCount) {
    Snd_SetPlayMode((short)seq, 0, playMode & 0xFF, (short)loopCount);
}

void SsSepPlay(long seq, long sep, long playMode, long loopCount) asm("func_80072628");
void SsSepPlay(long seq, long sep, long playMode, long loopCount) {
    Snd_SetPlayMode((short)seq, (short)sep, playMode & 0xFF, (short)loopCount);
}

void _SsSndSetVol(long seq, long sep, long left, long right) asm("func_80072660");
void _SsSndSetVol(long seq, long sep, long left, long right) {
    func_80076C58((short)(seq | (sep << 8)), (u_short)left, (u_short)right, 0);
}

void SsSeqSetVol(long seq, long left, long right) asm("func_80072698");
void SsSeqSetVol(long seq, long left, long right) {
    func_80076C58((short)seq, (u_short)left, (u_short)right, 0);
}

void SsSepSetVol(long seq, long sep, long left, long right) asm("func_800726C8");
void SsSepSetVol(long seq, long sep, long left, long right) {
    func_80076C58((short)(seq | (sep << 8)), (u_short)left, (u_short)right, 0);
}

long SpuVmGetSeqVol(long seq_sep, short *left, short *right) asm("func_80076DCC");

long SsSepGetVol(long seq, long sep, short *voll, short *volr) asm("func_80072700");
long SsSepGetVol(long seq, long sep, short *voll, short *volr) {
    return SpuVmGetSeqVol((short)(seq | (sep << 8)), voll, volr);
}

void func_80076ED8(long seqSep);

void _SsSndStop(short seq, short sep) {
    long i;
    SeqStruct *score;

    score = &g_SndSeqTable[seq][sep];
    g_SndSeqTable[seq][sep].flags &= ~1;
    g_SndSeqTable[seq][sep].flags &= ~2;
    g_SndSeqTable[seq][sep].flags &= ~8;
    g_SndSeqTable[seq][sep].flags |= 4;
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

void SsSeqStop(long seq) asm("func_800728A0");
void SsSeqStop(long seq) {
    _SsSndStop((short)seq, 0);
}

void SsSepStop(long seq, long sep) asm("func_800728C8");
void SsSepStop(long seq, long sep) {
    _SsSndStop((short)seq, (short)sep);
}
