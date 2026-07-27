#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void SsSeqSetNoteParam2C(long seq, long sep, u_char value) {
    SeqStruct *state = &D_801E79CC[(short)seq][(short)sep];

    state->programs[state->channel] = value;
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}
