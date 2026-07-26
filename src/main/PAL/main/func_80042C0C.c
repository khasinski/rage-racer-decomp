#include "common.h"

extern s32 g_CdCommandPending asm("D_8007F604");
extern s32 g_CdCommandStep asm("D_8007F60C");

void GamePauseCdAudio(void) asm("func_80042C0C");
void GamePauseCdAudio(void) {
    g_CdCommandPending = 2;
    g_CdCommandStep = 0;
}
