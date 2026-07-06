#include "psyq/snd.h"


extern SeqStruct *D_801E79CC[];

void SsSeqResetChannelNote(s32 seq, s32 sep) {
    SeqStruct *state = &D_801E79CC[(s16)seq][(s16)sep];

    SsUtReverbOff();
    SpuVmDamperOff();

    state->programs[state->channel] = state->channel;
    state->unk13 = 0;
    state->play_mode = 0;
    state->vol[state->channel] = 0x7F;
    state->panpot[state->channel] = 0x40;
    state->delta_value = SsSeqReadDeltaTime((s16)seq, (s16)sep);
}
