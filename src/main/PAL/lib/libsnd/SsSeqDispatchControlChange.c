#include "psyq/snd.h"

#include "psyq/snd_internal.h"

void SsSeqDispatchControlChange(short seq, short sep, long controller) {
    u_long control;
    short value;
    short channel;
    u_char *readPos;
    SeqStruct *state;

    control = (u8)controller;
    state = &g_SndSeqTable[seq][sep];
    readPos = state->read_pos;
    channel = state->channel;
    state->read_pos++;
    value = *readPos;

    switch (control) {
    case 0:
        state->vab_id = value;
        state->delta_value = SsSeqReadDeltaTime(seq, sep);
        return;
    case 6:
        ContDataEntry(seq, sep, value);
        return;
    case 7:
        SsSeqIndexChannel((sep << 8) | seq, state->vab_id,
                      state->programs[channel], value,
                      state->panpot[channel]);
        state->vol[channel] = value;
        state->delta_value = SsSeqReadDeltaTime(seq, sep);
        return;
    case 10:
        SsSeqIndexChannel((sep << 8) | seq, state->vab_id,
                      state->programs[channel], state->vol[channel],
                      value);
        state->panpot[channel] = value;
        state->delta_value = SsSeqReadDeltaTime(seq, sep);
        return;
    case 11:
        SsUtSetProgVol(state->vab_id, state->programs[channel], value);
        SsSeqIndexChannel((sep << 8) | seq, state->vab_id,
                      state->programs[channel], state->vol[channel],
                      state->panpot[channel]);
        state->delta_value = SsSeqReadDeltaTime(seq, sep);
        return;
    case 64:
        if (value < 64U) {
            SpuVmDamperOff();
        } else {
            SpuVmDamperOn();
        }
        state->delta_value = SsSeqReadDeltaTime(seq, sep);
        return;
    case 65:
        SsSeqSetPortamento(seq, sep, value);
        return;
    case 91:
        SsUtSetReverbDepth(value, value);
        state->delta_value = SsSeqReadDeltaTime(seq, sep);
        return;
    case 98:
        SsSeqApplyControlChange(seq, sep, value);
        return;
    case 99:
        SsSeqSetChannelMode(seq, sep, value);
        return;
    case 100:
        SsSeqSetChannelParam13(seq, sep, value);
        return;
    case 101:
        SsSeqSetChannelParam14(seq, sep, value);
        return;
    case 121:
        SsSeqResetChannelNote(seq, sep);
        return;
    default:
        state->delta_value = SsSeqReadDeltaTime(seq, sep);
        return;
    }
}
