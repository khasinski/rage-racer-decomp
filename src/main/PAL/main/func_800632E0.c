#include "common.h"
#include "game/random.h"

extern s32 D_8009B9A8;

void GameSeedRandom(s32 seed) {
    D_8009B9A8 = seed;
}
