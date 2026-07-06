#include "common.h"
#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];
extern void (*D_801E7A58[][0x10])(s32 seq, s32 sep, u8 value);

void SsSeqApplyControlChange(s32 seq, s32 sep, u8 value) asm("func_8006FDA8");

void SsSeqApplyControlChange(s32 seq, s32 sep, u8 value) {
    register s32 seq_raw asm("$17");
    register s32 sep_raw asm("$18");
    register s32 seq_shift asm("$4");
    register s32 sep_s asm("$5");
    register SeqStruct *state asm("$16");
    register s32 seq_offset asm("$4");
    register s32 sep_offset asm("$2");
    register SeqStruct *base asm("$3");

    seq_raw = seq;
    sep_raw = sep;
    asm("" : "=r"(seq_raw), "=r"(sep_raw) : "0"(seq_raw), "1"(sep_raw));
    sep_s = sep;
    seq_shift = seq << 16;
    seq_offset = seq_shift >> 14;
    sep_s = (s16)sep_s;
    sep_offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    base = *(SeqStruct **)((u8 *)D_801E79CC + seq_offset);
    sep_offset = sep_offset * 4;
    state = (SeqStruct *)(sep_offset + (s32)base);

    if (state->unk27 == 1) {
        if (state->unk10 == 0) {
            state->unk28 = value;
            state->unk10 = 1;
            goto maybe_callback;
        }
    }

    if (state->unk16 != 0x1E) {
        if (state->unk16 != 0x14) {
            state->unk15 = value;
            state->unk2a++;
        }
    }

maybe_callback:
    if (state->unk16 == 0x28) {
        register s32 seq_cb asm("$4");
        register s32 sep_cb asm("$5");
        register void (*callback)(s32, s32, u8) asm("$2");
        register s32 raw_shift asm("$2");

        seq_cb = seq_raw << 16;
        raw_shift = seq_raw << 16;
        seq_cb = raw_shift >> 16;
        callback = D_801E7A58[seq_cb][(s16)sep_raw];
        if (callback != 0) {
            callback(seq_cb, (s16)sep_raw, value);
        }
    }

read_delta:
    state->delta_value = SsSeqReadDeltaTime((s16)seq_raw, (s16)sep_raw);
}
