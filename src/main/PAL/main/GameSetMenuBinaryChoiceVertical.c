#include "common.h"
#include "game/audio.h"
#include "game/menu.h"



void GameSetMenuBinaryChoiceVertical(s32 *value) {
    u16 input = g_PadEdge;

    if (input & 0x8000) {
        if (*value == 0) {
            GamePlaySoundCue(1);
            *value = 1;
        } else {
            *value = 1;
        }
    } else if (input & 0x2000) {
        if (*value == 1) {
            GamePlaySoundCue(1);
        }
        *value = 0;
    }
}
