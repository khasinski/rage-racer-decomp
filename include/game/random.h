#ifndef GAME_RANDOM_H
#define GAME_RANDOM_H

#include "common.h"

s32 Random15(void) asm("func_800632B0");
void SeedRandom(s32 seed);

#endif
