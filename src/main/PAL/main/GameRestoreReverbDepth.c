#include "common.h"
#include "game/audio.h"

extern s32 D_801E6D84;
extern s32 D_801E6D88;

void GameRestoreReverbDepth(s32 arg0) {
    if (arg0 != 0) {
        GameSetReverbDepth(D_801E6D84, D_801E6D88);
    } else {
        GameSetReverbDepth(0, 0);
    }
}
