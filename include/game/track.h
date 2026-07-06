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
    s16 field_16;
} GameTrackPoint;

#endif
