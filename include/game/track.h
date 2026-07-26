#ifndef GAME_TRACK_H
#define GAME_TRACK_H

#include "common.h"

typedef struct GameTrackPoint {
    s32 x;
    s32 z;
    s16 y;
    s16 angle;
    u8 padC[4];
    s16 field_10;
    s16 field_12;
    u8 pad14[2];
    s16 segmentLength;
} GameTrackPoint;

/*
 * Track centreline points of the loaded course, g_TrackPointCount of them.
 * Walked cyclically by the car physics/AI and by the track renderer.
 */
extern GameTrackPoint *g_TrackPoints asm("D_8009E688");

/*
 * Number of valid entries in the track centreline point array g_TrackPoints.
 * Set by func_8002A6B0 when the track is built; every walker of the array
 * wraps its index with `% g_TrackPointCount`.
 */
extern s32 g_TrackPointCount asm("D_8009E6A8");

#endif
