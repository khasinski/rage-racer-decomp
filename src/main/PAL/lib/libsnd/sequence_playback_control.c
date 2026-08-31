#include "psyq/snd.h"

#include "psyq/snd_internal.h"

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
    state->play_count = loopCount;

    if (mode == 1) {
        playState = (SeqStruct *)(seqOffset + (long)*sequence);
        playState->flags |= 1;
        state->plays_started = 0;
        state->playing = 1;
        SpuVmSetSeqVol((short)(seq | (sep << 8)), state->left_volume, state->right_volume, 0);
    } else if (mode == 0) {
        stopState = (SeqStruct *)(seqOffset + (long)*sequence);
        stopState->flags |= 2;
    }
}

void SsSeqPlay(long seq, long playMode, long loopCount) {
    Snd_SetPlayMode((short)seq, 0, (u8)playMode, (short)loopCount);
}

void SsSepPlay(long seq, long sep, long playMode, long loopCount) {
    Snd_SetPlayMode((short)seq, (short)sep, (u8)playMode, (short)loopCount);
}

void _SsSndSetVol(long seq, long sep, long left, long right) {
    SpuVmSetSeqVol((short)(seq | (sep << 8)), (u_short)left, (u_short)right, 0);
}

void SsSeqSetVol(long seq, long left, long right) {
    SpuVmSetSeqVol((short)seq, (u_short)left, (u_short)right, 0);
}

void SsSepSetVol(long seq, long sep, long left, long right) {
    SpuVmSetSeqVol((short)(seq | (sep << 8)), (u_short)left, (u_short)right, 0);
}

long SsSepGetVol(long seq, long sep, short *voll, short *volr) {
    return SpuVmGetSeqVol((short)(seq | (sep << 8)), voll, volr);
}

void _SsSndStop(seq, sep)
    short seq;
    short sep;
{
    long i;
    SeqStruct *score;

    score = &g_SndSeqTable[seq][sep];
    g_SndSeqTable[seq][sep].flags &= ~1;
    g_SndSeqTable[seq][sep].flags &= ~2;
    g_SndSeqTable[seq][sep].flags &= ~8;
    g_SndSeqTable[seq][sep].flags |= 4;
    SpuVmSeqKeyOff((sep << 8) | seq);

    score->delta_value = score->base_delta_value;
    score->tempo = score->base_tempo;
    score->tick_period = score->tick_period_initial;
    score->read_pos = score->next_sep_pos;
    score->loop_pos = score->next_sep_pos;
    score->playing = 0;
    score->elapsed_ticks = 0;
    score->loop_marked = 0;
    score->rpn_param = 0;
    score->play_mode = 0;
    score->rpn_pending = 0;
    score->nrpn_lsb = 0;
    score->nrpn_msb = 0;
    score->nrpn_pending = 0;
    score->channel = 0;
    score->plays_started = 0;
    score->loop_marked = 0;
    score->loop_count = 0;
    score->loop_count_set = 0;
    score->running_status = 0;

    for (i = 0; i < 16; i++) {
        score->programs[i] = i;
        score->panpot[i] = 0x40;
        score->vol[i] = 0x7F;
    }

    score->cur_vol_left = 0x7F;
    score->cur_vol_right = 0x7F;
}

void SsSeqStop(long seq) {
    _SsSndStop((short)seq, 0);
}

void SsSepStop(long seq, long sep) {
    _SsSndStop((short)seq, (short)sep);
}
