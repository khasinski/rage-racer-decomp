#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void func_8006FB7C(s16 seq, s16 sep, u8 value) {
    SeqStruct *state;
    u8 channel;
    ProgAtr progAtr;
    VagAtr vagAtr;
    s32 tone;

    state = &D_801E79CC[seq][sep];
    channel = state->channel;
    SsUtGetProgAtr(state->unk4c, state->programs[channel], &progAtr);
    for (tone = 0; tone < progAtr.tones; tone++) {
        SsUtGetVagAtr(state->unk4c, state->programs[channel], (s16)tone, &vagAtr);
        if (value < 0x40) {
            vagAtr.mode = 2;
        } else if (value >= 0x40 && value < 0x80) {
            vagAtr.mode = 0;
        }
        SsUtSetVagAtr(state->unk4c, state->programs[channel], (s16)tone, &vagAtr);
    }

    state->delta_value = SsSeqReadDeltaTime(seq, sep);
}
