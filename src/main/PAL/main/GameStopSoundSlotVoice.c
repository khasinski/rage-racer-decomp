#include "common.h"
s32 SsUtKeyOffV(s32 voice) asm("func_80078018");
/* Stop the effect voice for sound slot `slot` (hardware voice = slot + 0xE). */
void GameStopSoundSlotVoice(s16 slot) asm("func_8005B360");
void GameStopSoundSlotVoice(s16 slot) { SsUtKeyOffV((s16)(slot + 14)); }
