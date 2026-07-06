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
    u8 pad2C[0x3C];
    s32 field_68;
    s32 field_6C;
    s32 field_70;
    u8 pad74[8];
    s16 velocityX;
    s16 velocityZ;
    s16 motionActive;
    u16 motionTimer;
    s16 motionMode;
    s16 motionModeTimer;
    s16 motionValue;
    s16 field_8A;
    s16 field_8C;
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
    s32 field_A4;
    u8 padA8[4];
    s16 activeFlag;
    s16 field_AE;
    u8 padB0[8];
    s16 routeRow;
    u8 padBA[2];
    s32 field_BC;
    u8 padC0[0x40];
    s32 routeIndex;
    u8 pad104[8];
    s16 field_10C;
    s16 field_10E;
    u8 pad110[0xC];
    s16 field_11C;
    u8 pad11E[0x12];
    s16 field_130;
    s16 field_132;
    s32 field_134;
    s16 field_138;
    s16 field_13A;
    s16 field_13C;
    s16 field_13E;
    u8 pad140[0xC];
    u8 field_14C;
    u8 field_14D;
    u8 field_14E;
    u8 field_14F;
    u8 pad150[0xC];
    s16 field_15C;
    u8 pad15E[0x3E];
} GameCarRuntime;

typedef struct GameCarRuntimeProgressWindow {
    s32 field_6C;
    u8 pad4[0x3C];
    s16 activeFlag;
    u8 pad42[0x15A];
} GameCarRuntimeProgressWindow;

typedef struct GameCarTrackAngleWindow {
    u8 pad0[0x30];
    s32 trackPointIndex;
    u8 pad34[0x6C];
    s32 headingAngle;
} GameCarTrackAngleWindow;

#endif
