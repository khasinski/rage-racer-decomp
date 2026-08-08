#ifndef GAME_PLAYER_CAR_INTERNAL_H
#define GAME_PLAYER_CAR_INTERNAL_H

#include "game/car.h"

#ifndef GAME_PLAYER_CAR_DECL
#define GAME_PLAYER_CAR_DECL extern GameCarRuntime g_PlayerCar
#endif
GAME_PLAYER_CAR_DECL;
#undef GAME_PLAYER_CAR_DECL
extern s32 g_PlayerTrackProgress;

#endif
