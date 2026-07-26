#include "common.h"
extern s32 D_80082F40;
void GameSetCdMixPreset(s32 arg0) asm("func_8004318C");
void func_80072AF4(void);
void GameSetStereoOutput(void) asm("func_8005BE24");
void GameSetStereoOutput(void) { D_80082F40 = 1; GameSetCdMixPreset(0); func_80072AF4(); }
