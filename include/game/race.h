#ifndef GAME_RACE_H
#define GAME_RACE_H

#include "common.h"

typedef struct GameRaceProgress {
    u8 pad0[0xC];
    s32 progression;
    s32 elapsedTime;
} GameRaceProgress;

#endif
