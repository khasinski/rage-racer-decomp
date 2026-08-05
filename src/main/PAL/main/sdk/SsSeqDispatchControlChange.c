#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

long func_80070D70(long seq, short sep);
void ContDataEntry(short seq, short sep, u_char value);
void SsSeqIndexChannel(long channel, short vab, u_char prog, short volume, long pan) asm("func_800771AC");
void func_8007701C(short vab, u_char prog, u_char volume);
void SpuVmDamperOff(void);
void SpuVmDamperOn(void);
void SsSeqSetPortamento(short seq, short sep, u_char value);
void func_80073748(long left, long right);
void func_8006FDA8(short seq, short sep, u_char value);
void func_8006FED8(short seq, short sep, u_char value);
void func_8007001C(short seq, short sep, u_char value);
void func_80070094(short seq, short sep, u_char value);
void func_8006FCE0(short seq, short sep);

void SsSeqDispatchControlChange(short seq, short sep, long arg2);
void SsSeqDispatchControlChange(short seq, short sep, long arg2) {
    u_long control;
    short value;
    short channel;
    u_char *readPos;
    SeqStruct *state;

    control = (u8)arg2;
    state = &g_SndSeqTable[seq][sep];
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
        ContDataEntry(seq, sep, value);
        return;
    case 7:
        SsSeqIndexChannel((sep << 8) | seq, state->unk4c,
                      state->programs[channel], value,
                      state->panpot[channel]);
        state->vol[channel] = value;
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 10:
        SsSeqIndexChannel((sep << 8) | seq, state->unk4c,
                      state->programs[channel], state->vol[channel],
                      value);
        state->panpot[channel] = value;
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 11:
        func_8007701C(state->unk4c, state->programs[channel], value);
        SsSeqIndexChannel((sep << 8) | seq, state->unk4c,
                      state->programs[channel], state->vol[channel],
                      state->panpot[channel]);
        state->delta_value = func_80070D70(seq, sep);
        return;
    case 64:
        if (value < 64U) {
            SpuVmDamperOff();
        } else {
            SpuVmDamperOn();
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
