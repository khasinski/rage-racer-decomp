#ifndef GAME_CAR_H
#define GAME_CAR_H

#include "common.h"

typedef struct GameCarEntry {
    u8 modelVariant;
    u8 pad1[2];
    u8 shapeIndex;
    u8 textureIndex;
    u8 enabled;
    u8 pad6[2];
} GameCarEntry;

typedef struct GameCarRuntime {
    s32 x;
    s16 y;
    u8 pad6[2];
    s32 z;
    u8 padC[0x14];
    s32 field_20;
    u8 pad24[4];
    s32 field_28;
    u8 pad2C[0x44];
    s32 trackPos0;
    s32 trackPos1;
    s16 bumpStepX;
    s16 bumpStepZ;
    s16 bumpActive;
    s16 bumpTimer;
    s16 effectMode;
    s16 effectTimer;
    s16 effectMagnitude;
    s16 field_8A;
    s16 steeringOffset;
    s16 field_8E;
    s16 field_90;
    s16 field_92;
    s16 field_94;
    s16 field_96;
    s16 field_98;
    s16 field_9A;
    s16 field_9C;
    s16 field_9E;
    u8 padA0[4];
    s32 trackDistance;
    u8 padA8[0x10];
    s16 routeRow;
    u8 padBA[2];
} GameCarRuntime;

#endif
