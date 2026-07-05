#include "common.h"
#include "game/audio.h"

void GameSetSoundSlotVoicesEnabled(s32 arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i != 5) {
            GameSetSoundSlotVoiceEnabled(i, arg0);
        }
    }
}
