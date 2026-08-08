#include "psyq/snd.h"

#include "psyq/snd_internal.h"

void func_8006F90C(long seq, long sep, long value) {
    register long seq_arg asm("$7");
    register long sep_arg asm("$8");
    long seq_raw;
    long sep_raw;
    register long value_raw asm("$23");
    SeqStruct *state;
    long channelStateRaw;
    SeqStruct *channelCopy;
    long i;
    VagAtr *vag;
    long seq_shift;
    register long seq_offset asm("$4");
    long sep_s;
    long sep_offset;
    long channel_offset;
    long tone_count;
    SeqStruct *base;
    ProgAtr *prog;
    ProgAtr progAttr;
    VagAtr vagAttr;
    volatile long framePad[2];

    seq_arg = seq;
    sep_arg = sep;
    asm("" : "=r"(seq_arg), "=r"(sep_arg) : "0"(seq_arg), "1"(sep_arg));
    value_raw = value;
    prog = &progAttr;
    asm("" : "=r"(value_raw), "=r"(prog) : "0"(value_raw), "1"(prog));
    seq_shift = seq_arg << 16;
    seq_offset = seq_shift >> 14;
    sep_s = sep_arg << 16;
    sep_s >>= 16;
    sep_offset = sep_s * 2;
    sep_offset += sep_s;
    sep_offset <<= 2;
    sep_offset -= sep_s;
    sep_offset <<= 2;
    sep_offset -= sep_s;
    sep_offset <<= 2;
    base = *(SeqStruct **)((u_char *)g_SndSeqTable + seq_offset);
    state = (SeqStruct *)(sep_offset + (long)base);
    seq_raw = seq_arg;
    channel_offset = state->channel;
    channelStateRaw = (long)state + channel_offset;
    sep_raw = sep_arg;
    SsUtGetProgAtr(state->unk4c, ((SeqStruct *)channelStateRaw)->programs[0], prog);

    tone_count = progAttr.tones;
    i = 0;
    if (tone_count > 0) {
        channelCopy = (SeqStruct *)channelStateRaw;
        vag = &vagAttr;
        do {
            channelStateRaw = i << 16;
            channelStateRaw >>= 16;
            SsUtGetVagAtr(state->unk4c, channelCopy->programs[0], channelStateRaw, vag);
            vagAttr.vibW = value_raw;
            SsUtSetVagAtr(state->unk4c, channelCopy->programs[0], channelStateRaw, vag);
            i++;
        } while (i < progAttr.tones);
    }

    state->delta_value = SsSeqReadDeltaTime((short)seq_raw, (short)sep_raw);
}

void func_8006FA44(long seq, long sep, long value) {
    register long seq_arg asm("$7");
    register long sep_arg asm("$8");
    long seq_raw;
    long sep_raw;
    register long value_raw asm("$23");
    SeqStruct *state;
    long channelStateRaw;
    SeqStruct *channelCopy;
    long i;
    VagAtr *vag;
    long seq_shift;
    register long seq_offset asm("$4");
    long sep_s;
    long sep_offset;
    long channel_offset;
    long tone_count;
    SeqStruct *base;
    ProgAtr *prog;
    ProgAtr progAttr;
    VagAtr vagAttr;
    volatile long framePad[2];

    seq_arg = seq;
    sep_arg = sep;
    asm("" : "=r"(seq_arg), "=r"(sep_arg) : "0"(seq_arg), "1"(sep_arg));
    value_raw = value;
    prog = &progAttr;
    asm("" : "=r"(value_raw), "=r"(prog) : "0"(value_raw), "1"(prog));
    seq_shift = seq_arg << 16;
    seq_offset = seq_shift >> 14;
    sep_s = sep_arg << 16;
    sep_s >>= 16;
    sep_offset = sep_s * 2;
    sep_offset += sep_s;
    sep_offset <<= 2;
    sep_offset -= sep_s;
    sep_offset <<= 2;
    sep_offset -= sep_s;
    sep_offset <<= 2;
    base = *(SeqStruct **)((u_char *)g_SndSeqTable + seq_offset);
    state = (SeqStruct *)(sep_offset + (long)base);
    seq_raw = seq_arg;
    channel_offset = state->channel;
    channelStateRaw = (long)state + channel_offset;
    sep_raw = sep_arg;
    SsUtGetProgAtr(state->unk4c, ((SeqStruct *)channelStateRaw)->programs[0], prog);

    tone_count = progAttr.tones;
    i = 0;
    if (tone_count > 0) {
        channelCopy = (SeqStruct *)channelStateRaw;
        vag = &vagAttr;
        do {
            channelStateRaw = i << 16;
            channelStateRaw >>= 16;
            SsUtGetVagAtr(state->unk4c, channelCopy->programs[0], channelStateRaw, vag);
            vagAttr.porT = value_raw;
            SsUtSetVagAtr(state->unk4c, channelCopy->programs[0], channelStateRaw, vag);
            i++;
        } while (i < progAttr.tones);
    }

    state->delta_value = SsSeqReadDeltaTime((short)seq_raw, (short)sep_raw);
}
