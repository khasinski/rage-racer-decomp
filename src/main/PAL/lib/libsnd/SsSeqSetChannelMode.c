#include "common.h"
#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

void SsSeqSetChannelMode(long seq, long sep, u_char mode) asm("func_8006FED8");

void SsSeqSetChannelMode(long seq, long sep, u_char mode) {
    long seq_raw;
    long sep_raw;
    /* These pins are load-bearing: removing any one changes .text. */
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
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(seq_raw), "=r"(sep_raw) : "0"(seq_raw), "1"(sep_raw));
    seq_s = (short)seq_s;
    seq_offset = seq_s * 4;
    sep_s = (short)sep_s;
    sep_offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    base = *(SeqStruct **)((u_char *)g_SndSeqTable + seq_offset);
    sep_offset = sep_offset * 4;
    state = (SeqStruct *)(sep_offset + (long)base);
    mode8 = (u_char)mode;

    if (mode8 == 0x14) {
        goto mode_14;
    }
    if (mode8 == 0x1E) {
        goto mode_1e;
    }
    goto mode_default;

mode_14:
    state->unk16 = mode;
    state->unk27 = 1;
    state->delta_value = SsSeqReadDeltaTime(seq_s, sep_s);
    state->loop_pos = state->read_pos;
    return;

mode_1e:
    state->unk16 = mode;
    if (state->unk28 == 0) {
        state->unk10 = 0;
        goto read_delta;
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

mode_default:
    {
        /* These pins are load-bearing: removing any one changes .text. */
        register long seq_arg asm("$4");
        register long sep_arg asm("$5");

        seq_arg = (short)seq_raw;
        sep_arg = (short)sep_raw;
        state->unk16 = mode;
        state->unk2a++;
read_delta:
        state->delta_value = SsSeqReadDeltaTime(seq_arg, sep_arg);
    }

done:
    return;
}
