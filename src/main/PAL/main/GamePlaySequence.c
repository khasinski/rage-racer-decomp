#include "common.h"
extern s16 g_SeqHandle asm("D_801E6D90");
void func_800725F0(s32 arg0, s32 arg1, s32 arg2);
void GamePlaySequence(void) asm("func_8005E88C");
void GamePlaySequence(void) { func_800725F0(g_SeqHandle, 1, 0); }
