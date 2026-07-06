#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void SsSeqSetNoteParam2C(s32 seq, s32 sep, u8 value) {
    SeqStruct *state = &D_801E79CC[(s16)seq][(s16)sep];

    state->programs[state->channel] = value;
    state->delta_value = SsSeqReadDeltaTime((s16)seq, (s16)sep);
}
