#ifndef GAME_RENDER_H
#define GAME_RENDER_H

#include "common.h"
#include "psyq/gte.h"

typedef struct GameRenderAxisMatrix {
    s16 m[3][3];
} GameRenderAxisMatrix;

typedef struct GameScratchpadRenderState {
    s32 pad0;
    void *primData;
    u8 pad8[0x14];
    s32 depth;
    u8 pad20[8];
    Matrix matrix;
    u8 pad48[0x20];
    s32 orderingFlag;
    s32 mode;
    u8 pad70[8];
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} GameScratchpadRenderState;

typedef struct GameRenderSourcePoint {
    u16 field_0;
    u8 pad2[2];
    u16 field_4;
    u8 pad6[2];
    u16 field_8;
    u8 padA[0x16];
    u16 field_20;
    u8 pad22[2];
    u16 field_24;
    u8 pad26[2];
    u16 field_28;
    u8 pad2A[6];
    u32 field_30;
    u8 pad34[0x10];
    u16 field_44;
    u8 pad46[2];
    u16 field_48;
    u8 pad4A[0x16];
    u16 field_60;
    u8 pad62[0x2A];
    s16 field_8C;
    u8 pad8E[0x20];
    s16 field_AE;
} GameRenderSourcePoint;

typedef struct GameRenderSinglePoint {
    u16 field_0;
    u16 field_2;
    u16 field_4;
    u16 field_6;
    u16 field_8;
    u16 field_A;
    u16 field_C;
    u16 field_E;
    s32 field_10;
    u32 field_14;
    u16 field_18;
    u8 pad1A[2];
} GameRenderSinglePoint;

typedef struct GameRenderPairPoint {
    u16 first_0;
    u16 first_2;
    u16 first_4;
    u16 first_6;
    u16 first_8;
    u16 first_A;
    u16 first_C;
    u16 first_E;
    u16 second_10;
    u16 second_12;
    u16 second_14;
    u16 second_16;
    u16 second_18;
    u16 second_1A;
    u16 second_1C;
    u16 second_1E;
    s32 field_20;
    u32 first_24;
    u32 second_28;
    u16 first_2C;
    u16 second_2E;
} GameRenderPairPoint;

#endif
