#include "common.h"
#include "game/audio.h"

void GameForceAllEffectVoicesEnabled(s32 arg0) {
    register s32 value asm("$16") = arg0;

    GameForcePanVoiceEnabled(value);
    GameForceBasicEffectVoicesEnabled(value);
    GameForceIndexedEffectVoiceEnabled(value);
    GameForcePitchEffectVoicesEnabled(value);
    GameForceSoundSlotVoicePlayback(value);
}
