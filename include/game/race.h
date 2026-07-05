#ifndef GAME_RACE_H
#define GAME_RACE_H

#include "common.h"

typedef struct GameRaceProgress {
    s32 state;
    s32 pad4;
    s32 lap;
    s32 progression;
    s32 elapsedTime;
} GameRaceProgress;

#endif
