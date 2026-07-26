#include "common.h"
#include "game/audio.h"
#include "game/menu.h"



u16 GamePollMenuConfirmInput(void) {
    u16 *state = &g_PadEdge2;
    u16 value;

    value = *state & 0x860;
    if (value != 0) {
        GamePlaySoundCue(2);
    }

    return *state & 0x860;
}
