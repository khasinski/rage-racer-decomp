#include "psyq/snd.h"


extern SeqStruct *D_801E79CC[];

void SsSeqSetChannelPitchBend(long seq, long sep, long arg2, long arg3) {
    long seq_raw = seq;
    long arg2_raw = arg2;
    long seq_offset = (seq_raw << 16) >> 14;
    long sep_s = (sep << 16) >> 16;
    long offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    SeqStruct *state = (SeqStruct *)((u_char *)*(SeqStruct **)((u_char *)D_801E79CC + seq_offset) + (offset * 4));
    u_char channel = state->channel;
    SeqStruct *channel_state = (SeqStruct *)((u_char *)state + channel);
    u_char pan = channel_state->panpot[0];
    long bend = arg3 & 0xFF;

    if (((state->padAA >> channel) & 1) == 0 && state->unk74 != 0) {
        if ((u_char)arg3 != 0) {
            SpuVmSeKeyOn((short)(seq_raw | (sep << 8)), state->unk4c, channel_state->programs[0], (u_char)arg2_raw, bend, pan);
            state->padA8 = bend;
        } else {
            SpuVmSeKeyOff((short)(seq_raw | (sep << 8)), state->unk4c, channel_state->programs[0], (u_char)arg2_raw);
        }
    }
}
