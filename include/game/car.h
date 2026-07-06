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
    s32 field_24;
    s32 field_28;
    u8 pad2C[4];
    s32 field_30;
    u8 pad34[0x34];
    s32 field_68;
    s32 field_6C;
    s32 field_70;
    u8 pad74[4];
    s16 field_78;
    s16 field_7A;
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
    s32 field_A0;
    s32 field_A4;
    s32 field_A8;
    s16 activeFlag;
    s16 field_AE;
    u8 padB0[8];
    s16 routeRow;
    u8 padBA[2];
    s32 field_BC;
    u8 padC0[0x30];
    s16 field_F0;
    s16 field_F2;
    s32 field_F4;
    u8 padF8[8];
    s32 routeIndex;
    u8 pad104[8];
    s16 field_10C;
    s16 field_10E;
    s16 field_110;
    s16 field_112;
    s16 field_114;
    s16 field_116;
    s16 field_118;
    s16 field_11A;
    s16 field_11C;
    s16 field_11E;
    s16 field_120;
    s16 field_122;
    s16 field_124;
    s16 field_126;
    s16 field_128;
    s16 field_12A;
    s16 field_12C;
    s16 field_12E;
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
    s16 field_15E;
    u8 pad160[0x3C];
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
