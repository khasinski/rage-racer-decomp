#include "common.h"
#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];
extern void (*g_SndMarkCallbacks[][0x10])(long seq, long sep, u_char value);

void SsSeqApplyControlChange(long seq, long sep, u_char value);

void SsSeqApplyControlChange(long seq, long sep, u_char value) {
    long seq_raw;
    long sep_raw;
    long seq_shift;
    long sep_s;
    SeqStruct *state;
    long seq_offset;
    long sep_offset;
    SeqStruct *base;

    seq_raw = seq;
    sep_raw = sep;
    asm("" : "=r"(seq_raw), "=r"(sep_raw) : "0"(seq_raw), "1"(sep_raw));
    sep_s = sep;
    seq_shift = seq << 16;
    seq_offset = seq_shift >> 14;
    sep_s = (short)sep_s;
    sep_offset = (((((sep_s * 2) + sep_s) * 4) - sep_s) * 4) - sep_s;
    base = *(SeqStruct **)((u_char *)g_SndSeqTable + seq_offset);
    sep_offset = sep_offset * 4;
    state = (SeqStruct *)(sep_offset + (long)base);

    switch (0) { default:
    if (state->unk27 == 1) {
        if (state->unk10 == 0) {
            state->unk28 = value;
            state->unk10 = 1;
            break;
        }
    }

    if (state->unk16 != 0x1E) {
        if (state->unk16 != 0x14) {
            state->unk15 = value;
            state->unk2a++;
        }
    }

    }
    if (state->unk16 == 0x28) {
        long seq_cb;
        long sep_cb;
        void (*callback)(long, long, u_char);
        register long raw_shift asm("$2");

        seq_cb = seq_raw << 16;
        raw_shift = seq_raw << 16;
        seq_cb = raw_shift >> 16;
        callback = g_SndMarkCallbacks[seq_cb][(short)sep_raw];
        if (callback != 0) {
            callback(seq_cb, (short)sep_raw, value);
        }
    }

    state->delta_value = SsSeqReadDeltaTime((short)seq_raw, (short)sep_raw);
}
