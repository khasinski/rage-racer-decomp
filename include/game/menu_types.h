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

enum MenuPalette {
    MENU_PAINT_COLOR_COUNT = 18
};

/* Layout of the 0x18-byte engine specification block in a car model asset. */
typedef struct CarEngineSpec {
    u8 reserved00[0x10];
    s16 maxPower;
    s16 maxPowerRpm;
    u8 maxTorqueFraction;
    u8 maxTorqueWhole;
    s16 maxTorqueRpm;
} CarEngineSpec;

/* 6x6 mask selecting the DESIGN MODE script cells to draw. */
typedef struct DesignModeCellMask {
    u8 cells[6][6];
} DesignModeCellMask;

/* Screen-space output produced while flipping the course card. */
typedef struct MenuProjectedVertex {
    u16 x;
    u16 y;
    u16 z;
    u16 pad;
} MenuProjectedVertex;

/* Texture coordinates for the four glyph cells of a NeGcon setup panel. */
typedef struct NegconUvTemplate {
    u8 uv[8];
} NegconUvTemplate;

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
