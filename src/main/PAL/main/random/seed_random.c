#include "common.h"
#include "game/random.h"

extern s32 g_RandomSeed;

void SeedRandom(s32 seed) {
    g_RandomSeed = seed;
}
