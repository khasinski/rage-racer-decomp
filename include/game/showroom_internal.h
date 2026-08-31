#ifndef GAME_SHOWROOM_INTERNAL_H
#define GAME_SHOWROOM_INTERNAL_H

#include "common.h"
#include "game/car.h"
#include "game/vector.h"

typedef struct ShowroomCarPose {
    s32 position[4];
    s32 unk10[4];
    Vec4 rotation;
} ShowroomCarPose;

/*
 * The showroom reuses the player car block as its own turntable state, so
 * g_PlayerCar is declared here as a union over the three views the code wants
 * and as a plain PlayerCarRuntime in player_car_internal.h. Both spellings are
 * load-bearing: gcc 2.6.3 treats a struct member reference as non-aliasing, so
 * folding the showroom onto casts of the runtime struct moves the code.
 */
typedef union ShowroomPlayerCarState {
    ShowroomCarPose pose;
    s32 courseViewX;
    PlayerCarRuntime runtime;
} ShowroomPlayerCarState;

extern ShowroomPlayerCarState g_PlayerCar;

#endif
