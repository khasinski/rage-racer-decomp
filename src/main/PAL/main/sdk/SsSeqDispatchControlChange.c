#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

s32 func_80070D70(s32 seq, s16 sep);
void func_8007010C(s16 seq, s16 sep, u8 value);
void func_800771AC(s32 channel, s16 vab, u8 prog, s16 volume, s32 pan);
void func_8007701C(s16 vab, u8 prog, u8 volume);
void func_800731A8(void);
void func_800731B8(void);
void SsSeqSetPortamento(s16 seq, s16 sep, u8 value) asm("func_8006FB7C");
void func_80073748(s32 left, s32 right);
void func_8006FDA8(s16 seq, s16 sep, u8 value);
void func_8006FED8(s16 seq, s16 sep, u8 value);
void func_8007001C(s16 seq, s16 sep, u8 value);
void func_80070094(s16 seq, s16 sep, u8 value);
void func_8006FCE0(s16 seq, s16 sep);

void SsSeqDispatchControlChange(s16 seq, s16 sep, s32 arg2) asm("func_8006F5F4");
void SsSeqDispatchControlChange(s16 seq, s16 sep, s32 arg2) {
    u32 control;
    s16 value;
    s16 channel;
    u8 *readPos;
    SeqStruct *state;

    control = arg2 & 0xFF;
    state = &D_801E79CC[seq][sep];
    readPos = state->read_pos;
    channel = state->channel;
    state->read_pos++;
    value = *readPos;

    switch (control) {
    case 0:
        state->unk4c = value;
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 6:
        func_8007010C(seq, sep, value);
        return;
    case 7:
        func_800771AC((sep << 8) | seq, state->unk4c,
                      state->programs[channel], value,
                      state->panpot[channel]);
        state->vol[channel] = value;
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 10:
        func_800771AC((sep << 8) | seq, state->unk4c,
                      state->programs[channel], state->vol[channel],
                      value);
        state->panpot[channel] = value;
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 11:
        func_8007701C(state->unk4c, state->programs[channel], value);
        func_800771AC((sep << 8) | seq, state->unk4c,
                      state->programs[channel], state->vol[channel],
                      state->panpot[channel]);
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 64:
        if (value < 64U) {
            func_800731A8();
        } else {
            func_800731B8();
        }
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 65:
        SsSeqSetPortamento(seq, sep, value);
        return;
    case 91:
        func_80073748(value, value);
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 98:
        func_8006FDA8(seq, sep, value);
        return;
    case 99:
        func_8006FED8(seq, sep, value);
        return;
    case 100:
        func_8007001C(seq, sep, value);
        return;
    case 101:
        func_80070094(seq, sep, value);
        return;
    case 121:
        func_8006FCE0(seq, sep);
        return;
    default:
        state->delta_value = func_80070D70(seq, sep);
        return;
    }
}
