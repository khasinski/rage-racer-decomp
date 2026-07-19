#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void func_8006F5F4(s32 seq, s32 sep, s32 value);
void func_80070A1C(s32 seq, s32 sep, s32 value);

void func_8006F1E0(s16 seq, s16 sep) {
    SeqStruct *state = &D_801E79CC[seq][sep];
    u8 *p;
    s32 status;
    s32 lo;
    s32 d1;

    p = state->read_pos;
    state->read_pos = p + 1;
    status = *p;
    if (status & 0x80) {
        lo = status & 0xF;
        state->channel = lo;
        switch (status & 0xF0) {
        case 0x90:
            state->unk11 = 0x90;
            status = *state->read_pos++;
            d1 = *state->read_pos++;
            state->delta_value = SsSeqReadDeltaTime(seq, sep);
            SsSeqSetChannelPitchBend(seq, sep, status, d1);
            break;
        case 0xB0:
            state->unk11 = 0xB0;
            status = *state->read_pos++;
            func_8006F5F4(seq, sep, status);
            break;
        case 0xC0:
            state->unk11 = 0xC0;
            status = *state->read_pos++;
            SsSeqSetNoteParam2C(seq, sep, status);
            break;
        case 0xE0:
            state->unk11 = 0xE0;
            state->read_pos++;
            SsSeqApplyProgramChange(seq, sep);
            break;
        case 0xF0:
            state->unk11 = 0xFF;
            state->channel = lo;
            status = *state->read_pos++;
            func_80070A1C(seq, sep, status);
            break;
        }
    } else {
        switch (state->unk11) {
        case 0x90:
            state->read_pos = p + 2;
            d1 = p[1];
            state->delta_value = SsSeqReadDeltaTime(seq, sep);
            SsSeqSetChannelPitchBend(seq, sep, status, d1);
            break;
        case 0xB0:
            func_8006F5F4(seq, sep, status);
            break;
        case 0xC0:
            SsSeqSetNoteParam2C(seq, sep, status);
            break;
        case 0xE0:
            SsSeqApplyProgramChange(seq, sep);
            break;
        case 0xFF:
            func_80070A1C(seq, sep, status);
            break;
        }
    }
}
