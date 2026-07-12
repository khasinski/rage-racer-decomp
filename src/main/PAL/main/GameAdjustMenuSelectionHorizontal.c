#include "common.h"
#include "game/audio.h"
#include "game/menu.h"



void GameAdjustMenuSelectionHorizontal(s32 *value, s32 min, s32 max) {
    u16 input = g_PadEdge;
    s32 next;

    if (input & 0x4000) {
        next = *value + 1;
        *value = next;
        if (max < next) {
            *value = max;
            return;
        }
    } else if (input & 0x1000) {
        next = *value - 1;
        *value = next;
        if (next < min) {
            *value = min;
            return;
        }
    } else {
        return;
    }

    GamePlaySoundCue(1);
}
