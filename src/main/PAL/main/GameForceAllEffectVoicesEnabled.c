#include "common.h"
#include "game/audio.h"

void GameForceAllEffectVoicesEnabled(s32 arg0) {
    GameForcePanVoiceEnabled(arg0);
    GameForceBasicEffectVoicesEnabled(arg0);
    GameForceIndexedEffectVoiceEnabled(arg0);
    GameForcePitchEffectVoicesEnabled(arg0);
    GameForceSoundSlotVoicePlayback(arg0);
}
