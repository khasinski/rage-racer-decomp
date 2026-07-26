#include "common.h"
extern s32 D_80082F40;
void GameSetCdMixPreset(s32 arg0) asm("func_8004318C");
void func_80072AE0(void);
void GameSetMonoOutput(void) asm("func_8005BE58");
void GameSetMonoOutput(void) { D_80082F40 = 0; GameSetCdMixPreset(1); func_80072AE0(); }
