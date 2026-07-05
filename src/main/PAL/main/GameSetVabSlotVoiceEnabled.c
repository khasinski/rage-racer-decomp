#include "common.h"
#include "game/audio.h"

extern s32 D_801E6CC8[];

void GameSetVabSlotVoiceEnabled(s32 arg0, s32 arg1, s32 arg2) {
    s32 *state;

    if (arg1 != 0) {
        s32 *base = D_801E6CC8;

        state = base + arg0;
        if (*state == 0) {
            GameStartVabSlotVoice(arg0, 0, (s16)arg2);
            *state = 1;
        }
    } else {
        s32 *base = D_801E6CC8;

        state = base + arg0;
        if (*state != 0) {
            GameStopDirectVoice(arg0);
            *state = 0;
        }
    }
}
