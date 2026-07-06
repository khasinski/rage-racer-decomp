#include "common.h"
#include "game/audio.h"
#include "game/menu.h"

extern u16 D_801E436E;


u16 GamePollMenuBackInput(void) {
    u16 *state = &D_801E436E;
    u16 value;

    value = *state & 0x90;
    if (value != 0) {
        GamePlaySoundCue(3);
    }

    return *state & 0x90;
}
