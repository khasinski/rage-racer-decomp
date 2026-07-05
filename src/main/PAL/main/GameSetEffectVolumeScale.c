#include "common.h"
#include "game/audio.h"

extern s32 D_801E6CA4;

void GameSetEffectVolumeScale(s32 arg0) {
    if (arg0 >= 0) {
        if (arg0 < 0x81) {
            goto done;
        }
        arg0 = 0x80;
    } else {
        arg0 = 0;
    }

done:
    D_801E6CA4 = arg0;
}
