#include "common.h"

void func_80078018(s32 arg0);

void GameStopDirectVoice(s16 voice) asm("func_8005E6F4");
void GameStopDirectVoice(s16 voice) {
    func_80078018(voice);
}
