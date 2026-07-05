#include "common.h"
#include "game/audio.h"

extern s32 D_801E6D8C;
extern s32 D_801E6D9C;

void GameStartSequenceFadeOut(void) {
    D_801E6D9C = -4;
    D_801E6D8C = -3;
}
