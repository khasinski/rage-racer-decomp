#include "common.h"
void func_80078018(s32 arg0);
/* Stop the effect voice for sound slot `slot` (hardware voice = slot + 0xE). */
void GameStopSoundSlotVoice(s16 slot) asm("func_8005B360");
void GameStopSoundSlotVoice(s16 slot) { func_80078018((s16)(slot + 14)); }
