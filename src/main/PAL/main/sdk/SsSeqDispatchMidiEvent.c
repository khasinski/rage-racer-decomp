#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

void SsSeqDispatchControlChange(long seq, long sep, long value) asm("func_8006F5F4");
void SsSeqSetChannelProgram(long seq, long sep, long value) asm("func_80070A1C");

void SsSeqDispatchMidiEvent(short seq, short sep) asm("func_8006F1E0");
void SsSeqDispatchMidiEvent(short seq, short sep) {
    SeqStruct *state = &g_SndSeqTable[seq][sep];
    u_char *p;
    long status;
    long lo;
    long d1;

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
            SsSeqDispatchControlChange(seq, sep, status);
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
            SsSeqSetChannelProgram(seq, sep, status);
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
            SsSeqDispatchControlChange(seq, sep, status);
            break;
        case 0xC0:
            SsSeqSetNoteParam2C(seq, sep, status);
            break;
        case 0xE0:
            SsSeqApplyProgramChange(seq, sep);
            break;
        case 0xFF:
            SsSeqSetChannelProgram(seq, sep, status);
            break;
        }
    }
}
