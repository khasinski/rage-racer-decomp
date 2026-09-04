#ifndef GAME_WAYPOINT_H
#define GAME_WAYPOINT_H

#include "common.h"
#include "game/vector.h"

typedef struct TrackWaypointSeed {
    s32 x;
    s32 y;
    s16 stepX;
    s16 stepY;
} TrackWaypointSeed;

typedef struct TrackWaypointMotion {
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
} TrackWaypointMotion;

typedef struct TrackWaypointRuntime {
    s32 active;
    TrackWaypointMotion motion;
} TrackWaypointRuntime;

typedef char TrackWaypointSeedSizeCheck[
    sizeof(TrackWaypointSeed) == 0xC ? 1 : -1];
typedef char TrackWaypointMotionSizeCheck[
    sizeof(TrackWaypointMotion) == 0x34 ? 1 : -1];
typedef char TrackWaypointRuntimeSizeCheck[
    sizeof(TrackWaypointRuntime) == 0x38 ? 1 : -1];

#endif
