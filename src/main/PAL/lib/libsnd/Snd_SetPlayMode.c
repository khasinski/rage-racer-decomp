#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

void SpuVmSetSeqVol(long seq_sep, u_short left, u_short right, long mode);

void Snd_SetPlayMode(long seq, long sep, long playMode, long loopCount);
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
    mode = (u8)playMode;
    flagsState->flags &= ~4;
    state->unk46 = loopCount;

    if (mode == 1) {
        playState = (SeqStruct *)(seqOffset + (long)*sequence);
        playState->flags |= 1;
        state->unk48 = 0;
        state->unk2b = 1;
        SpuVmSetSeqVol((short)(seq | (sep << 8)), state->left_volume, state->right_volume, 0);
    } else if (mode == 0) {
        stopState = (SeqStruct *)(seqOffset + (long)*sequence);
        stopState->flags |= 2;
    }
}

void SsSeqPlay(long seq, long playMode, long loopCount);
void SsSeqPlay(long seq, long playMode, long loopCount) {
    Snd_SetPlayMode((short)seq, 0, (u8)playMode, (short)loopCount);
}

void SsSepPlay(long seq, long sep, long playMode, long loopCount);
void SsSepPlay(long seq, long sep, long playMode, long loopCount) {
    Snd_SetPlayMode((short)seq, (short)sep, (u8)playMode, (short)loopCount);
}

void _SsSndSetVol(long seq, long sep, long left, long right);
void _SsSndSetVol(long seq, long sep, long left, long right) {
    SpuVmSetSeqVol((short)(seq | (sep << 8)), (u_short)left, (u_short)right, 0);
}

void SsSeqSetVol(long seq, long left, long right);
void SsSeqSetVol(long seq, long left, long right) {
    SpuVmSetSeqVol((short)seq, (u_short)left, (u_short)right, 0);
}

void SsSepSetVol(long seq, long sep, long left, long right);
void SsSepSetVol(long seq, long sep, long left, long right) {
    SpuVmSetSeqVol((short)(seq | (sep << 8)), (u_short)left, (u_short)right, 0);
}

long SpuVmGetSeqVol(long seq_sep, short *left, short *right);

long SsSepGetVol(long seq, long sep, short *voll, short *volr);
long SsSepGetVol(long seq, long sep, short *voll, short *volr) {
    return SpuVmGetSeqVol((short)(seq | (sep << 8)), voll, volr);
}

void SpuVmSeqKeyOff(long seqSep);

void _SsSndStop(short seq, short sep) {
    long i;
    SeqStruct *score;

    score = &g_SndSeqTable[seq][sep];
    g_SndSeqTable[seq][sep].flags &= ~1;
    g_SndSeqTable[seq][sep].flags &= ~2;
    g_SndSeqTable[seq][sep].flags &= ~8;
    g_SndSeqTable[seq][sep].flags |= 4;
    SpuVmSeqKeyOff((sep << 8) | seq);

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

void SsSeqStop(long seq);
void SsSeqStop(long seq) {
    _SsSndStop((short)seq, 0);
}

void SsSepStop(long seq, long sep);
void SsSepStop(long seq, long sep) {
    _SsSndStop((short)seq, (short)sep);
}
