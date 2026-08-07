#ifndef GAME_MENU_TYPES_H
#define GAME_MENU_TYPES_H

#include "common.h"
#include "game/vector.h"

typedef struct ScoreRecord {
    s16 place;
    u16 clears;
} ScoreRecord;

typedef struct RaceRecord {
    s8 driverName[8];
    s32 raceTime;
    s16 carIndex;
    s16 unused;
} RaceRecord;

typedef struct TeamLogoSample {
    u16 clut[2][16];
    u16 canvas[64][16];
} TeamLogoSample;

typedef struct PaintColorTable {
    Rgb colors[18];
} PaintColorTable;

typedef struct ClassRecordSprite {
    u8 u1;
    u8 v1;
    u8 u2;
    u8 v2;
    u16 clut1;
    u16 clut2;
    u16 clut3;
    u16 unused;
} ClassRecordSprite;

#endif
