#include "common.h"
extern s16 g_SeqHandle asm("D_801E6D90");
void func_800728A0(s32 arg0);
void GameStopSequence(void) asm("func_8005E8B8");
void GameStopSequence(void) { func_800728A0(g_SeqHandle); }
