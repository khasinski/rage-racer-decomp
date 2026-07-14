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

typedef struct GameRenderView {
    s16 angle_0;    /* 0x00 */
    s16 angle_2;    /* 0x02 */
    s16 angle_4;    /* 0x04 */
    s16 horizon_6;  /* 0x06 */
} GameRenderView;

typedef struct GameRenderObject {
    s32 x;          /* 0x00 */
    s32 y;          /* 0x04 */
    s32 z;          /* 0x08 */
    s32 field_0C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 angle_20;   /* 0x20 */
    s32 angle_24;   /* 0x24 */
    s32 angle_28;   /* 0x28 */
    s32 field_2C;
    s32 field_30;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 angle_44;   /* 0x44 */
    s32 flags_48;   /* 0x48 (bit 0x1000) */
    s32 field_4C;
    s32 angle_50;   /* 0x50 */
    s32 angle_54;   /* 0x54 */
    s32 angle_58;   /* 0x58 */
    s32 field_5C;
    s32 field_60;   /* 0x60 */
    s32 field_64;   /* 0x64 */
    s32 field_68;
    s32 field_6C;
    void *field_70; /* 0x70 */
    s32 field_74;
    s32 field_78;
    s32 field_7C;
    s32 field_80;
    s32 field_84;
    s32 field_88;
    s32 field_8C;
    s32 field_90;
    s32 field_94;
    s32 field_98;
    s32 field_9C;
    s32 field_A0;
    s32 field_A4;
    s32 field_A8;
    s32 field_AC;
    s32 field_B0;
    s32 field_B4;
    s32 field_B8;
    s32 field_BC;
    s32 field_C0;
    s32 field_C4;
    s32 field_C8;
    s32 field_CC;
    s32 field_D0;
    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;
    s32 field_E4;   /* 0xE4 */
} GameRenderObject;

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
