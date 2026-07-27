#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

long SsSeqReadDeltaTime(long arg0, long arg1) asm("func_80070D70");

long SsSeqReadDeltaTime(long arg0, long arg1) {
    SeqStruct *state;
    u_char *stream;
    long value;
    long byte;
    long ret;

    state = &D_801E79CC[(short)arg0][(short)arg1];

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
