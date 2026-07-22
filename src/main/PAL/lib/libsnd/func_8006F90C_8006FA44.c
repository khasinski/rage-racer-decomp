#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void func_8006F90C(s32 seq, s32 sep, s32 value) {
    register s32 seq_arg asm("$7");
    register s32 sep_arg asm("$8");
    register s32 seq_raw asm("$21");
    register s32 sep_raw asm("$22");
    register s32 value_raw asm("$23");
    register SeqStruct *state asm("$18");
    register s32 channelStateRaw asm("$16");
    register SeqStruct *channelCopy asm("$20");
    register s32 i asm("$17");
    register VagAtr *vag asm("$19");
    register s32 seq_shift asm("$4");
    register s32 seq_offset asm("$4");
    register s32 sep_s asm("$3");
    register s32 sep_offset asm("$2");
    register s32 channel_offset asm("$2");
    register s32 tone_count asm("$2");
    register SeqStruct *base asm("$3");
    register ProgAtr *prog asm("$6");
    ProgAtr progAttr;
    VagAtr vagAttr;
    volatile s32 framePad[2];

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
    base = *(SeqStruct **)((u8 *)D_801E79CC + seq_offset);
    state = (SeqStruct *)(sep_offset + (s32)base);
    seq_raw = seq_arg;
    channel_offset = state->channel;
    channelStateRaw = (s32)state + channel_offset;
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

    state->delta_value = SsSeqReadDeltaTime((s16)seq_raw, (s16)sep_raw);
}

void func_8006FA44(s32 seq, s32 sep, s32 value) {
    register s32 seq_arg asm("$7");
    register s32 sep_arg asm("$8");
    register s32 seq_raw asm("$21");
    register s32 sep_raw asm("$22");
    register s32 value_raw asm("$23");
    register SeqStruct *state asm("$18");
    register s32 channelStateRaw asm("$16");
    register SeqStruct *channelCopy asm("$20");
    register s32 i asm("$17");
    register VagAtr *vag asm("$19");
    register s32 seq_shift asm("$4");
    register s32 seq_offset asm("$4");
    register s32 sep_s asm("$3");
    register s32 sep_offset asm("$2");
    register s32 channel_offset asm("$2");
    register s32 tone_count asm("$2");
    register SeqStruct *base asm("$3");
    register ProgAtr *prog asm("$6");
    ProgAtr progAttr;
    VagAtr vagAttr;
    volatile s32 framePad[2];

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
    base = *(SeqStruct **)((u8 *)D_801E79CC + seq_offset);
    state = (SeqStruct *)(sep_offset + (s32)base);
    seq_raw = seq_arg;
    channel_offset = state->channel;
    channelStateRaw = (s32)state + channel_offset;
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

    state->delta_value = SsSeqReadDeltaTime((s16)seq_raw, (s16)sep_raw);
}
