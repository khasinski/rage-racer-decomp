#include "psyq/snd.h"
#include "common.h"

#include "psyq/snd_internal.h"

void SsSeqSetPortamento(short seq, short sep, u_char value) {
    SeqStruct *state;
    u_char channel;
    ProgAtr progAtr;
    VagAtr vagAtr;
    long tone;

    state = &g_SndSeqTable[seq][sep];
    channel = state->channel;
    SsUtGetProgAtr(state->vab_id, state->programs[channel], &progAtr);
    for (tone = 0; tone < progAtr.tones; tone++) {
        SsUtGetVagAtr(state->vab_id, state->programs[channel], (short)tone, &vagAtr);
        if (value < 0x40) {
            vagAtr.mode = 2;
        } else if (value >= 0x40 && value < 0x80) {
            vagAtr.mode = 0;
        }
        SsUtSetVagAtr(state->vab_id, state->programs[channel], (short)tone, &vagAtr);
    }

    state->delta_value = SsSeqReadDeltaTime(seq, sep);
}

void SsSeqResetChannelNote(long seq, long sep) {
    SeqStruct *state = &g_SndSeqTable[(short)seq][(short)sep];

    SsUtReverbOff();
    SpuVmDamperOff();

    state->programs[state->channel] = state->channel;
    state->rpn_param = 0;
    state->play_mode = 0;
    state->vol[state->channel] = 0x7F;
    state->panpot[state->channel] = 0x40;
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}

void SsSeqApplyControlChange(long seq, long sep, u_char value) {
    long seq_raw;
    long sep_raw;
    long seq_shift;
    long sep_s;
    SeqStruct *state;
    long seq_offset;
    long sep_offset;
    SeqStruct *base;

    seq_raw = seq;
    sep_raw = sep;
    asm("" : "=r"(seq_raw), "=r"(sep_raw) : "0"(seq_raw), "1"(sep_raw));
    sep_s = sep;
    seq_shift = seq << 16;
    seq_offset = seq_shift >> 14;
    sep_s = (short)sep_s;
    sep_offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    base = *(SeqStruct **)((u_char *)g_SndSeqTable + seq_offset);
    sep_offset = sep_offset * 4;
    state = (SeqStruct *)(sep_offset + (long)base);

    switch (0) { default:
    if (state->loop_marked == 1) {
        if (state->loop_count_set == 0) {
            state->loop_count = value;
            state->loop_count_set = 1;
            break;
        }
    }

    if (state->nrpn_msb != 0x1E) {
        if (state->nrpn_msb != 0x14) {
            state->nrpn_lsb = value;
            state->nrpn_pending++;
        }
    }

    }
    if (state->nrpn_msb == 0x28) {
        long seq_cb;
        long sep_cb;
        void (*callback)(long, long, u_char);
        long raw_shift = seq_raw << 16;

        seq_cb = seq_raw << 16;
        seq_cb = raw_shift >> 16;
        callback = g_SndMarkCallbacks[seq_cb][(short)sep_raw];
        if (callback != 0) {
            callback(seq_cb, (short)sep_raw, value);
        }
    }

    state->delta_value = SsSeqReadDeltaTime((short)seq_raw, (short)sep_raw);
}

void SsSeqSetChannelMode(long seq, long sep, u_char mode) {
    long seq_raw;
    long sep_raw;
    register long seq_s asm("$4");
    register long sep_s asm("$5");
    SeqStruct *state;
    register long seq_address asm("$3");
    long sep_offset;
    long mode8;

    seq_raw = seq;
    sep_raw = sep;
    seq_s = seq;
    sep_s = sep;
    asm("" : "=r"(seq_raw), "=r"(sep_raw) : "0"(seq_raw), "1"(sep_raw));
    seq_s = (short)seq_s;
    seq_address = seq_s * 4;
    sep_s = (short)sep_s;
    sep_offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    seq_address = *(long *)((u_char *)g_SndSeqTable + seq_address);
    sep_offset = sep_offset * 4;
    state = (SeqStruct *)(sep_offset + seq_address);
    mode8 = (u_char)mode;

    switch (mode8) {
    case 0x14:
    state->nrpn_msb = mode;
    state->loop_marked = 1;
    state->delta_value = SsSeqReadDeltaTime(seq_s, sep_s);
    state->loop_pos = state->read_pos;
    return;

    case 0x1E:
    state->nrpn_msb = mode;
    if (state->loop_count == 0) {
        state->loop_count_set = 0;
        break;
    }
    if (state->loop_count < 0x7F) {
        state->loop_count--;
        state->delta_value = SsSeqReadDeltaTime(seq_s, sep_s);
        if (state->loop_count != 0) {
            state->read_pos = state->loop_pos;
        } else {
            state->loop_count_set = 0;
        }
        return;
    }
    SsSeqReadDeltaTime(seq_s, sep_s);
    state->delta_value = 0;
    state->read_pos = state->loop_pos;
    return;

    default:
        seq_s = (short)seq_raw;
        sep_s = (short)sep_raw;
        state->nrpn_msb = mode;
        state->nrpn_pending++;
        break;
    }
    state->delta_value = SsSeqReadDeltaTime(seq_s, sep_s);

    return;
}

void SsSeqSetChannelParam13(long seq, long sep, u_char value) {
    SeqStruct *state = &g_SndSeqTable[(short)seq][(short)sep];

    state->rpn_param = value;
    state->rpn_pending++;
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}

void SsSeqSetChannelParam14(long seq, long sep, u_char value) {
    SeqStruct *state = &g_SndSeqTable[(short)seq][(short)sep];

    state->play_mode = value;
    state->rpn_pending++;
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}
