#include "common.h"

s32 SsUtKeyOffV(s32 voice) asm("func_80078018");

void GameStopDirectVoice(s16 voice) asm("func_8005E6F4");
void GameStopDirectVoice(s16 voice) {
    SsUtKeyOffV(voice);
}
