#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

s32 SsSeqReadDeltaTime(s32 arg0, s32 arg1) asm("func_80070D70");

s32 SsSeqReadDeltaTime(s32 arg0, s32 arg1) {
    SeqStruct *state;
    u8 *stream;
    s32 value;
    s32 byte;
    s32 ret;

    state = &D_801E79CC[(s16)arg0][(s16)arg1];

    stream = state->read_pos;
    state->read_pos = stream + 1;
    value = stream[0];

    if (value == 0) {
        return 0;
    }

    if (value & 0x80) {
        value &= 0x7F;
        do {
            stream = state->read_pos;
            state->read_pos = stream + 1;
            byte = stream[0];
            value = (value << 7) + (byte & 0x7F);
        } while (byte & 0x80);
    }

    ret = value * 10;
    state->unk80 += ret;
    return ret;
}
