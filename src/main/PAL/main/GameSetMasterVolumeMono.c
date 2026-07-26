#include "common.h"
void func_8006EAFC(s32 arg0, s32 arg1);
void GameSetMasterVolumeMono(s16 arg0) asm("func_8005EAA8");
void GameSetMasterVolumeMono(s16 arg0) { func_8006EAFC(arg0, arg0); }
