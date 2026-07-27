#include "common.h"
#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];
extern void (*D_801E7A58[][0x10])(long seq, long sep, u_char value);

void SsSeqApplyControlChange(long seq, long sep, u_char value) asm("func_8006FDA8");

void SsSeqApplyControlChange(long seq, long sep, u_char value) {
    register long seq_raw asm("$17");
    register long sep_raw asm("$18");
    register long seq_shift asm("$4");
    register long sep_s asm("$5");
    register SeqStruct *state asm("$16");
    register long seq_offset asm("$4");
    register long sep_offset asm("$2");
    register SeqStruct *base asm("$3");

    seq_raw = seq;
    sep_raw = sep;
    asm("" : "=r"(seq_raw), "=r"(sep_raw) : "0"(seq_raw), "1"(sep_raw));
    sep_s = sep;
    seq_shift = seq << 16;
    seq_offset = seq_shift >> 14;
    sep_s = (short)sep_s;
    sep_offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    base = *(SeqStruct **)((u_char *)D_801E79CC + seq_offset);
    sep_offset = sep_offset * 4;
    state = (SeqStruct *)(sep_offset + (long)base);

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
        register long seq_cb asm("$4");
        register long sep_cb asm("$5");
        register void (*callback)(long, long, u_char) asm("$2");
        register long raw_shift asm("$2");

        seq_cb = seq_raw << 16;
        raw_shift = seq_raw << 16;
        seq_cb = raw_shift >> 16;
        callback = D_801E7A58[seq_cb][(short)sep_raw];
        if (callback != 0) {
            callback(seq_cb, (short)sep_raw, value);
        }
    }

read_delta:
    state->delta_value = SsSeqReadDeltaTime((short)seq_raw, (short)sep_raw);
}
