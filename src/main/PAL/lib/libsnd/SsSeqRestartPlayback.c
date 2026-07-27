#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");

void SsSeqRestartPlayback(short arg0, short arg1) asm("func_80070F10");

void SsSeqRestartPlayback(short arg0, short arg1) {
    SeqStruct *state;

    state = &g_SndSeqTable[arg0][arg1];
    state->unk46 = 1;
    state->unk48 = 0;

    g_SndSeqTable[arg0][arg1].flags &= ~0x100;
    g_SndSeqTable[arg0][arg1].flags &= ~0x8;
    g_SndSeqTable[arg0][arg1].flags &= ~0x2;
    g_SndSeqTable[arg0][arg1].flags &= ~0x4;
    g_SndSeqTable[arg0][arg1].flags &= ~0x200;

    state->read_pos = state->next_sep_pos;
    state->unk2b = 1;
    g_SndSeqTable[arg0][arg1].flags |= 1;
}
