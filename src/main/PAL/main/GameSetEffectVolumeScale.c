#include "common.h"
#include "game/audio.h"


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
    g_EffectVolumeScale = arg0;
}
