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

typedef union TrackWaypointVelocity {
    Vec4 vector;
    struct {
        s32 x;
        s32 reservedX;
        s32 y;
        s32 reservedY;
    } fields;
} TrackWaypointVelocity;

typedef struct TrackWaypointMotion {
    s32 x;
    s32 height;
    s32 y;
    s32 field10;
    s32 rotationZ;
    s32 rotationY;
    s32 field1C;
    s32 field20;
    TrackWaypointVelocity velocity;
    s32 velocityMagnitude;
} TrackWaypointMotion;

typedef struct TrackWaypointRuntime {
    s32 active;
    TrackWaypointMotion motion;
} TrackWaypointRuntime;

#endif
