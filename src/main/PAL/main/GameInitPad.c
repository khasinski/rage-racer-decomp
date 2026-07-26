#include "common.h"
#include "game/state.h"

extern u8 D_801E403C[];

void InitPad(void *buf0, s32 len0, void *buf1, s32 len1) asm("func_800631F0");
void StartPad(void) asm("func_80063200");

/* BIOS InitPAD over the two 0x28-byte pad buffers, then StartPAD. */
void GameInitPad(void) {
    InitPad(D_801E403C, 0x28, D_801E403C + 0x28, 0x28);
    StartPad();
}
