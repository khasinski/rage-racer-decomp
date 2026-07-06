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
    s32 code;
    s32 y;
    u8 pad10[4];
    s32 deltaY;
    s32 fixedStep;
    s32 deltaX;
    u8 pad20[0x18];
} TrackWaypointRuntime;

#endif
