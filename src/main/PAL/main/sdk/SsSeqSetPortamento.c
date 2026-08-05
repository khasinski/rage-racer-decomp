#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];

void SsSeqSetPortamento(short seq, short sep, u_char value) {
    SeqStruct *state;
    u_char channel;
    ProgAtr progAtr;
    VagAtr vagAtr;
    long tone;

    state = &g_SndSeqTable[seq][sep];
    channel = state->channel;
    SsUtGetProgAtr(state->unk4c, state->programs[channel], &progAtr);
    for (tone = 0; tone < progAtr.tones; tone++) {
        SsUtGetVagAtr(state->unk4c, state->programs[channel], (short)tone, &vagAtr);
        if (value < 0x40) {
            vagAtr.mode = 2;
        } else if (value >= 0x40 && value < 0x80) {
            vagAtr.mode = 0;
        }
        SsUtSetVagAtr(state->unk4c, state->programs[channel], (short)tone, &vagAtr);
    }

    state->delta_value = SsSeqReadDeltaTime(seq, sep);
}
