#include "psyq/snd.h"


extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

void SsSeqApplyProgramChange(long seq, long sep) {
    SeqStruct *state = &g_SndSeqTable[(short)seq][(short)sep];
    u_char *read_pos = state->read_pos;

    state->read_pos++;
    SpuVmApplyPitchBendByTone(((short)sep << 8) | (short)seq, state->unk4c, *((u_char *)((long)state->channel + (long)state + 0x2C)), *read_pos);
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}
