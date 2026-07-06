#include "common.h"
#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void SsSeqSetChannelMode(s32 seq, s32 sep, u8 mode) asm("func_8006FED8");

void SsSeqSetChannelMode(s32 seq, s32 sep, u8 mode) {
    register s32 seq_raw asm("$7");
    register s32 sep_raw asm("$8");
    register s32 seq_s asm("$4");
    register s32 sep_s asm("$5");
    register SeqStruct *state asm("$16");
    register s32 seq_offset asm("$3");
    register s32 sep_offset asm("$2");
    register SeqStruct *base asm("$3");
    s32 mode8;

    seq_raw = seq;
    sep_raw = sep;
    seq_s = seq;
    sep_s = sep;
    asm("" : "=r"(seq_raw), "=r"(sep_raw) : "0"(seq_raw), "1"(sep_raw));
    seq_s = (s16)seq_s;
    seq_offset = seq_s * 4;
    sep_s = (s16)sep_s;
    sep_offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    base = *(SeqStruct **)((u8 *)D_801E79CC + seq_offset);
    sep_offset = sep_offset * 4;
    state = (SeqStruct *)(sep_offset + (s32)base);
    mode8 = (u8)mode;

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
    goto done;

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
            asm("" : "=r"(state) : "0"(state));
            state->unk10 = 0;
        }
        goto done;
    }
    SsSeqReadDeltaTime(seq_s, sep_s);
    state->delta_value = 0;
    state->read_pos = state->loop_pos;
    goto done;

mode_default:
    {
        register s32 seq_arg asm("$4");
        register s32 sep_arg asm("$5");

        seq_arg = (s16)seq_raw;
        sep_arg = (s16)sep_raw;
        state->unk16 = mode;
        state->unk2a++;
        asm("" : : : "memory");
read_delta:
        state->delta_value = SsSeqReadDeltaTime(seq_arg, sep_arg);
    }

done:
    return;
}
