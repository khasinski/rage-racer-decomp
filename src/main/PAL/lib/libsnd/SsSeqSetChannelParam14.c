#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void SsSeqSetChannelParam14(long seq, long sep, u_char value) {
    SeqStruct *state = &D_801E79CC[(short)seq][(short)sep];

    state->play_mode = value;
    state->unk29++;
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}
