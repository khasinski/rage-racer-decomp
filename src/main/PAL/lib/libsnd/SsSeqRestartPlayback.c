#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];

void SsSeqRestartPlayback(s16 arg0, s16 arg1) asm("func_80070F10");

void SsSeqRestartPlayback(s16 arg0, s16 arg1) {
    SeqStruct *state;

    state = &D_801E79CC[arg0][arg1];
    state->unk46 = 1;
    state->unk48 = 0;

    D_801E79CC[arg0][arg1].flags &= ~0x100;
    D_801E79CC[arg0][arg1].flags &= ~0x8;
    D_801E79CC[arg0][arg1].flags &= ~0x2;
    D_801E79CC[arg0][arg1].flags &= ~0x4;
    D_801E79CC[arg0][arg1].flags &= ~0x200;

    state->read_pos = state->next_sep_pos;
    state->unk2b = 1;
    D_801E79CC[arg0][arg1].flags |= 1;
}
