#include "common.h"
#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];

void SsSeqSetChannelMode(long seq, long sep, u_char mode) asm("func_8006FED8");

void SsSeqSetChannelMode(long seq, long sep, u_char mode) {
    long seq_raw;
    long sep_raw;
    register long seq_s asm("$4");
    register long sep_s asm("$5");
    SeqStruct *state;
    register long seq_offset asm("$3");
    long sep_offset;
    register SeqStruct *base asm("$3");
    long mode8;

    seq_raw = seq;
    sep_raw = sep;
    seq_s = seq;
    sep_s = sep;
    asm("" : "=r"(seq_raw), "=r"(sep_raw) : "0"(seq_raw), "1"(sep_raw));
    seq_s = (short)seq_s;
    seq_offset = seq_s * 4;
    sep_s = (short)sep_s;
    sep_offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    base = *(SeqStruct **)((u_char *)g_SndSeqTable + seq_offset);
    sep_offset = sep_offset * 4;
    state = (SeqStruct *)(sep_offset + (long)base);
    mode8 = (u_char)mode;

    switch (mode8) {
    case 0x14:
    state->unk16 = mode;
    state->unk27 = 1;
    state->delta_value = SsSeqReadDeltaTime(seq_s, sep_s);
    state->loop_pos = state->read_pos;
    return;

    case 0x1E:
    state->unk16 = mode;
    if (state->unk28 == 0) {
        state->unk10 = 0;
        break;
    }
    if (state->unk28 < 0x7F) {
        state->unk28--;
        state->delta_value = SsSeqReadDeltaTime(seq_s, sep_s);
        if (state->unk28 != 0) {
            state->read_pos = state->loop_pos;
        } else {
            state->unk10 = 0;
        }
        return;
    }
    SsSeqReadDeltaTime(seq_s, sep_s);
    state->delta_value = 0;
    state->read_pos = state->loop_pos;
    return;

    default:
        seq_s = (short)seq_raw;
        sep_s = (short)sep_raw;
        state->unk16 = mode;
        state->unk2a++;
        break;
    }
    state->delta_value = SsSeqReadDeltaTime(seq_s, sep_s);

    return;
}
