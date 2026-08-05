#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];

void SsSeqSetChannelParam14(long seq, long sep, u_char value) {
    SeqStruct *state = &g_SndSeqTable[(short)seq][(short)sep];

    state->play_mode = value;
    state->unk29++;
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}
