#ifndef GAME_RENDER_H
#define GAME_RENDER_H

#include "common.h"
#include "psyq/gte.h"

typedef struct GameRenderAxisMatrix {
    s16 m[3][3];
} GameRenderAxisMatrix;

/*
 * Camera / horizon row. Base D_8019C9A8, indexed +8*screen. `horizon` is added
 * to / subtracted from a GameRenderObject's y during the view transform.
 */
typedef struct CamRow {
    u8 pad0[0xC];
    s16 axis0;   /* 0x0C */
    u16 axis1;   /* 0x0E */
    u16 axis2;   /* 0x10 */
    s16 horizon; /* 0x12 */
} CamRow;

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

/*
 * 2D/HUD primitive emitters. All of them pack the primitive at the scratchpad
 * cursor `*(u8 **)0x1F800000`, bump that cursor past the primitive and link it
 * into the ordering table `ot` with AddPrim. Sprites and textured quads turn a
 * linear CLUT index into VRAM clut coordinates (20 cluts per row, first row at
 * y = 0x1E0). Where an `alpha` argument exists, 0xFF means opaque; anything
 * else enables semi-transparency and appends a blend packet via func_80017390.
 */
void GameDrawSprite(
    void *ot,
    s16 x0,
    s16 y0,
    s16 w,
    u16 h,
    u16 u0,
    u16 v0,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    s32 shadeTex,
    s32 semiTrans,
    u32 flags) asm("func_80046A2C");
void GameDrawFlatTriangle(
    void *ot,
    s16 x0,
    s16 y0,
    s16 x1,
    u16 y1,
    u16 x2,
    u16 y2,
    u8 r,
    u8 g,
    u8 b,
    s32 semiTrans,
    u32 flags) asm("func_80046BA0");
void GameDrawFlatQuad(
    void *ot,
    s16 x0,
    s16 y0,
    s16 x1,
    u16 y1,
    u16 x2,
    u16 y2,
    u16 x3,
    u16 y3,
    u8 r,
    u8 g,
    u8 b,
    s32 semiTrans,
    u32 flags) asm("func_80046CBC");
/* POLY_FT4: four xy/uv pairs, flat rgb, tpage and a CLUT index as depth key. */
void GameDrawTexturedQuad(
    s32 ot,
    s16 x0,
    s16 y0,
    s16 x1,
    u16 y1,
    u16 x2,
    u16 y2,
    u16 x3,
    u16 y3,
    u8 u0,
    u8 v0,
    u8 u1,
    u8 v1,
    u8 u2,
    u8 v2,
    u8 u3,
    u8 v3,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    s32 shadeTex,
    s32 semiTrans,
    u16 tpage) asm("func_80046E00");
/* TILE: solid rectangle at (x, y) sized (w, h). */
void GameDrawSolidRect(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    u16 h,
    u8 r,
    u8 g,
    u8 b,
    u8 alpha) asm("func_80047024");
void GameDrawLine(
    void *ot,
    s32 x0,
    s32 y0,
    s32 x1,
    u16 y1,
    u8 r,
    u8 g,
    u8 b,
    u8 alpha) asm("func_8004711C");
/* LINE_F3: flat-shaded 3-point polyline. */
void GameDrawPolyLine3(
    void *ot,
    s16 x0,
    s16 y0,
    s16 x1,
    s16 y1,
    s16 x2,
    s16 y2,
    u8 r,
    u8 g,
    u8 b,
    u8 alpha) asm("func_80047214");
/* LINE_G2: line interpolating rgb0 -> rgb1. */
void GameDrawGradientLine(
    void *ot,
    s32 x0,
    s32 y0,
    s32 x1,
    u16 y1,
    u8 r0,
    u8 g0,
    u8 b0,
    u8 r1,
    u8 g1,
    u8 b1,
    u8 alpha) asm("func_80047330");
/* Two-pixel-thick rectangle border, built from six GameDrawLine calls. */
void GameDrawRectOutline(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    u8 r,
    u8 g,
    u8 b,
    u8 alpha) asm("func_80047460");
/* Clips (x, y, w, h) to the 320x480 frame and queues a SetDrawArea packet. */
void GameSetDrawClipRect(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    s32 h) asm("func_800468FC");

/*
 * Text and number output, both built on GameDrawSprite. The two fonts differ
 * only in cell size and glyph table: small = 6x12 (D_8007F984), large = 8x16
 * (D_8007FA3C). Bit 0x80 of `flags` selects fixed-width cells instead of the
 * per-glyph widths in the table.
 */
void GameDrawSmallText(
    s32 x,
    s16 y,
    u8 *str,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    s32 flags) asm("func_80047634");
void GameDrawLargeText(
    s32 x,
    s16 y,
    u8 *str,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    s32 flags) asm("func_80047958");
/* Right-aligned decimal, up to 10 digits, leading zeros blanked. Returns the
 * number of digit sprites emitted. flags: 1 = large font, 4 = fixed digits at
 * v=0xDC, 8 = draw into the overlay OT layer. */
s32 GameDrawNumber(
    s32 x,
    s16 y,
    s32 flags,
    u32 value,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    u8 primitiveCount) asm("func_80047BD4");
/* Blits an 8x6 bit pattern from D_8007F6E8 as 4x8 blocks; negative argument
 * animates through the table. */
void GameDrawBitPatternOverlay(s32 pattern) asm("func_80047E60");

/*
 * Timed draw script: a table of {time, type, arg0, arg1} entries replayed
 * against a progress counter, terminated by time < 0. Element types 0/1/9 draw
 * a sprite, 10/19 a line, 20/29 a triangle and 30/39 a textured quad; the +9
 * variants are skipped while D_8019CB0C is set. Each element interpolates its
 * position from a packed s16 velocity pair by (elapsed * velocity) >> 5.
 * Returns 1 once the progress counter has reached the terminator's limit.
 */
s32 GameRunTimedDrawScript(
    void *commands,
    s32 *progress,
    s32 step) asm("func_800487D8");
void GameDrawScriptedSprite(
    s32 elapsed,
    u8 *style,
    u8 *record,
    s32 useAlpha) asm("func_80048078");
void GameDrawScriptedLine(s32 elapsed, u8 *style, u8 *record) asm("func_80048210");
void GameDrawScriptedTriangle(s32 elapsed, u8 *style, u8 *record) asm("func_800483D4");
void GameDrawScriptedQuad(s32 elapsed, u8 *style, s32 *record) asm("func_80048580");

#endif
