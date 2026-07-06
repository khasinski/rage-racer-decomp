#ifndef GAME_WAYPOINT_H
#define GAME_WAYPOINT_H

#include "common.h"

typedef struct TrackWaypointSeed {
    s32 x;
    s32 y;
    s16 stepX;
    s16 stepY;
} TrackWaypointSeed;

typedef struct TrackWaypointRuntime {
    s32 active;
    s32 x;
    s32 height;
    s32 y;
    s32 field10;
    s32 scale;
    s32 field18;
    s32 field1C;
    s32 field20;
    s32 velocityX;
    s32 field28;
    s32 velocityY;
    s32 field30;
    s32 velocityMagnitude;
} TrackWaypointRuntime;

#endif
