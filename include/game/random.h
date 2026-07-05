#ifndef GAME_RANDOM_H
#define GAME_RANDOM_H

#include "common.h"

s32 GameRandom15(void) asm("func_800632B0");
void GameSeedRandom(s32 seed) asm("func_800632E0");

#endif
