#include "psyq/snd.h"


extern SeqStruct *g_SndSeqTable[];

void SsSeqResetChannelNote(long seq, long sep) {
    SeqStruct *state = &g_SndSeqTable[(short)seq][(short)sep];

    SsUtReverbOff();
    SpuVmDamperOff();

    state->programs[state->channel] = state->channel;
    state->unk13 = 0;
    state->play_mode = 0;
    state->vol[state->channel] = 0x7F;
    state->panpot[state->channel] = 0x40;
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}
