#include "common.h"
#include "game/audio.h"

extern s32 D_801E6D98;

void GameRefreshSequenceVolumeScale(void) {
    s32 temp = D_801E6D98 * 114;

    GameSetSequenceVolume(temp / 15);
}

void GameSetSequenceVolumeScale(s32 arg0) {
    s32 temp = arg0 * 114;

    D_801E6D98 = arg0;
    GameSetSequenceVolume(temp / 15);
}
