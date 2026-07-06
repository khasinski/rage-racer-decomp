#include "psyq/snd.h"


extern SeqStruct *D_801E79CC[];

void SsSeqSetChannelPitchBend(s32 seq, s32 sep, s32 arg2, s32 arg3) {
    s32 seq_raw = seq;
    s32 arg2_raw = arg2;
    s32 seq_offset = (seq_raw << 16) >> 14;
    s32 sep_s = (sep << 16) >> 16;
    s32 offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    SeqStruct *state = (SeqStruct *)((u8 *)*(SeqStruct **)((u8 *)D_801E79CC + seq_offset) + (offset * 4));
    u8 channel = state->channel;
    SeqStruct *channel_state = (SeqStruct *)((u8 *)state + channel);
    u8 pan = channel_state->panpot[0];
    s32 bend = arg3 & 0xFF;

    if (((state->padAA >> channel) & 1) == 0 && state->unk74 != 0) {
        if ((u8)arg3 != 0) {
            SpuVmSeKeyOn((s16)(seq_raw | (sep << 8)), state->unk4c, channel_state->programs[0], (u8)arg2_raw, bend, pan);
            state->padA8 = bend;
        } else {
            SpuVmSeKeyOff((s16)(seq_raw | (sep << 8)), state->unk4c, channel_state->programs[0], (u8)arg2_raw);
        }
    }
}
