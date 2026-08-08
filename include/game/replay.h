#ifndef GAME_REPLAY_H
#define GAME_REPLAY_H

#include "common.h"

typedef struct ReplayCarState {
    s32 x;
    s32 y;
    s32 z;
    u8 pad0C[0x14];
    s32 rotX;
    s32 rotY;
    s32 rotZ;
    u8 pad2C[4];
    s32 field30;
    u8 pad34[0x10];
    s32 field44;
    s32 flags48;
    u8 pad4C[0x14];
    s32 z2;
    u8 pad64[0x28];
    s16 field8C;
    u8 pad8E[0x20];
    s16 variantAE;
} ReplayCarState;

typedef struct ReplayGrandPrixFrame {
    u16 x0;
    s16 y0;
    u16 z0;
    s16 z20;
    s16 rotX0;
    s16 rotY0;
    s16 rotZ0;
    s16 flags0;
    u16 x1;
    s16 y1;
    u16 z1;
    s16 z21;
    s16 rotX1;
    s16 rotY1;
    s16 rotZ1;
    s16 flags1;
    s32 field20;
    s32 field30_0;
    s32 field30_1;
    s16 field44_0;
    s16 field44_1;
} ReplayGrandPrixFrame;

typedef struct ReplayTimeAttackFrame {
    u16 x;
    s16 y;
    u16 z;
    s16 z2;
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    s16 flags;
    s32 field10;
    s32 field30;
    s16 field44;
} ReplayTimeAttackFrame;

#endif
