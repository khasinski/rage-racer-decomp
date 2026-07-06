#include "psyq/snd.h"


extern SeqStruct *D_801E79CC[];

void SsSeqApplyProgramChange(s32 seq, s32 sep) {
    SeqStruct *state = &D_801E79CC[(s16)seq][(s16)sep];
    u8 *read_pos = state->read_pos;

    state->read_pos++;
    SpuVmApplyPitchBendByTone(((s16)sep << 8) | (s16)seq, state->unk4c, *((u8 *)((s32)state->channel + (s32)state + 0x2C)), *read_pos);
    state->delta_value = SsSeqReadDeltaTime((s16)seq, (s16)sep);
}
