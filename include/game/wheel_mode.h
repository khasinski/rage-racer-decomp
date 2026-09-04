#ifndef GAME_WHEEL_MODE_H
#define GAME_WHEEL_MODE_H

#include "common.h"
#include "game/vector.h"

typedef struct WheelModeSeed {
    s32 x;
    s32 y;
    s16 stepX;
    s16 stepY;
} WheelModeSeed;

typedef struct WheelModeWheelMotion {
    s32 x;
    s32 height;
    s32 y;
    s32 reserved0C;
    /* The wheel models use the retail Z/Y/X angle order. */
    s32 rotationZ;
    s32 rotationY;
    s32 rotationX;
    s32 reserved1C;
    /* Horizontal track motion uses the source Vec4's X/Z components. */
    Vec4 velocity;
    s32 velocityMagnitude;
} WheelModeWheelMotion;

typedef struct WheelModeWheel {
    s32 active;
    WheelModeWheelMotion motion;
} WheelModeWheel;

typedef char WheelModeSeedSizeCheck[
    sizeof(WheelModeSeed) == 0xC ? 1 : -1];
typedef char WheelModeWheelMotionSizeCheck[
    sizeof(WheelModeWheelMotion) == 0x34 ? 1 : -1];
typedef char WheelModeWheelSizeCheck[
    sizeof(WheelModeWheel) == 0x38 ? 1 : -1];

#endif
