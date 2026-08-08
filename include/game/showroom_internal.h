#ifndef GAME_SHOWROOM_INTERNAL_H
#define GAME_SHOWROOM_INTERNAL_H

#include "common.h"
#include "game/car.h"
#include "game/vector.h"

typedef struct ShowroomCarModel {
    s32 unk0;
    u16 height;
} ShowroomCarModel;

typedef struct ShowroomCarPose {
    s32 position[4];
    s32 unk10[4];
    Vec4 rotation;
} ShowroomCarPose;

typedef union ShowroomPlayerCarState {
    ShowroomCarPose pose;
    s32 courseViewX;
    PlayerCarRuntime runtime;
} ShowroomPlayerCarState;

extern ShowroomPlayerCarState g_PlayerCar;

#endif
