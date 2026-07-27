#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

void SsSeqSetChannelParam13(long seq, long sep, u_char value) {
    SeqStruct *state = &g_SndSeqTable[(short)seq][(short)sep];

    state->unk13 = value;
    state->unk29++;
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}
