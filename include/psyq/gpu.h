#ifndef RAGE_PC_PSYQ_GPU_H
#define RAGE_PC_PSYQ_GPU_H

#include "common.h"

typedef struct Rect {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

typedef struct GpuRectPacked {
    u32 xy;
    s16 w;
    s16 h;
} GpuRectPacked;

typedef struct GpuTexWindow {
    u8 x;
    u8 pad1;
    u8 y;
    u8 pad3;
    s16 w;
    s16 h;
} GpuTexWindow;

typedef struct DispEnv {
    Rect disp;
    Rect screen;
    u8 isinter;
    u8 isrgb24;
} DispEnv;

typedef struct DrawEnv {
    Rect clip;
    s16 ofs[2];
    Rect tw;
    u16 tpage;
    u8 dtd;
    u8 dfe;
} DrawEnv;

/*
 * The libgpu primitive packets the game builds, in PSY-Q layout: a 4-byte
 * ordering-table tag that AddPrim links, then the packed command word (rgb of
 * vertex 0 + the primitive code the Set* helpers stamp in).
 */
typedef struct P_TAG {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
} P_TAG;

/* Flat triangle, 0x14 bytes. func_80064EE0 = SetPolyF3. */
typedef struct POLY_F3 {
    P_TAG t;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
} POLY_F3;

/* Flat quad, 0x18 bytes. func_80064F30 = SetPolyF4. */
typedef struct POLY_F4 {
    P_TAG t;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
} POLY_F4;

/* Textured quad, 0x28 bytes. func_80064F44 = SetPolyFT4. */
typedef struct POLY_FT4 {
    P_TAG t;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    s16 x1;
    s16 y1;
    u8 u1;
    u8 v1;
    u16 tpage;
    s16 x2;
    s16 y2;
    u8 u2;
    u8 v2;
    u16 pad1E;
    s16 x3;
    s16 y3;
    u8 u3;
    u8 v3;
    u16 pad26;
} POLY_FT4;

/* Textured sprite, 0x14 bytes. func_80064FA8 = SetSprt. */
typedef struct SPRT {
    P_TAG t;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    s16 w;
    s16 h;
} SPRT;

/* Solid rectangle, 0x10 bytes. func_80064FF8 = SetTile. */
typedef struct TILE {
    P_TAG t;
    s16 x0;
    s16 y0;
    s16 w;
    s16 h;
} TILE;

/* Flat line, 0x10 bytes. func_8006500C = SetLineF2. */
typedef struct LINE_F2 {
    P_TAG t;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} LINE_F2;

/* Flat 3-point polyline, 0x18 bytes. func_80065034 = SetLineF3. */
typedef struct LINE_F3 {
    P_TAG t;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    u32 pad14;
} LINE_F3;

/* Gradient line, 0x14 bytes. func_80065020 = SetLineG2. */
typedef struct LINE_G2 {
    P_TAG t;
    s16 x0;
    s16 y0;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 pad0F;
    s16 x1;
    s16 y1;
} LINE_G2;

typedef struct DrawPacket {
    u8 pad0[3];
    u8 code;
    u32 x0y0;
    u32 x1y1;
} DrawPacket;

/*
 * The libgpu driver table at 0x800941A0 (D_800941E0 points at it), dumped in
 * asm/PAL/main/data/main/6BE64.data.s. Slots holding a `u32` are worker
 * function addresses passed to `send` rather than called directly:
 *   +0x04 func_8006767C  +0x08 Gpu_AddQueue      +0x0C Gpu_ClearImage
 *   +0x10 Gpu_WriteGp1   +0x14 Gpu_WriteGp0Words +0x18 Gpu_StartDmaTransfer
 *   +0x1C Gpu_StoreImage +0x20 Gpu_LoadImage     +0x24 Gpu_ExecuteQueue
 *   +0x28 Gpu_GetControlMirrorByte               +0x2C Gpu_ClearOTagDma
 *   +0x30 _param         +0x34 Gpu_Reset         +0x38 _status
 *   +0x3C Gpu_DrawSync
 */
typedef struct GpuCallbacks {
    u8 pad0[0x8];
    s32 (*send)(u32 worker, void *buf, s32 size, u32 data);
    u32 cmd0C;
    void (*submit)(s32 cmd);
    void (*writeGp0Words)(void *src, s32 count);
    u32 sendList;
    u32 storeImage;
    u32 loadImage;
    u8 pad24[0x28 - 0x24];
    s32 (*read)(s32 cmd);
    void (*clearOTag)(void *ot, s32 count);
    u8 pad30[0x34 - 0x30];
    void (*resetGraph)(s32 mode);
    s32 (*status)(void);
    void (*drawSync)(s32 mode);
} GpuCallbacks;

/*
 * libgpu primitive initialisers. Each stamps the word count into prim[3] and
 * the GPU command byte into prim[7] (see the GP0 opcode in the comment).
 */
void SetPolyF3(void *prim) asm("func_80064EE0");   /* 0x20 */
void SetPolyFT3(void *prim) asm("func_80064EF4");  /* 0x24 */
void SetPolyG3(void *prim) asm("func_80064F08");   /* 0x30 */
void SetPolyGT3(void *prim) asm("func_80064F1C");  /* 0x34 */
void SetPolyF4(void *prim) asm("func_80064F30");   /* 0x28 */
void SetPolyFT4(void *prim) asm("func_80064F44");  /* 0x2C */
void SetPolyG4(void *prim) asm("func_80064F58");   /* 0x38 */
void SetPolyGT4(void *prim) asm("func_80064F6C");  /* 0x3C */
void SetSprt8(void *prim) asm("func_80064F80");    /* 0x74 */
void SetSprt16(void *prim) asm("func_80064F94");   /* 0x7C */
void SetSprt(void *prim) asm("func_80064FA8");     /* 0x64 */
void SetTile1(void *prim) asm("func_80064FBC");    /* 0x68 */
void SetTile8(void *prim) asm("func_80064FD0");    /* 0x70 */
void SetTile16(void *prim) asm("func_80064FE4");   /* 0x78 */
void SetTile(void *prim) asm("func_80064FF8");     /* 0x60 */
void SetLineF2(void *prim) asm("func_8006500C");   /* 0x40 */
void SetLineG2(void *prim) asm("func_80065020");   /* 0x50 */
void SetLineF3(void *prim) asm("func_80065034");   /* 0x48 */
void SetLineG3(void *prim) asm("func_80065054");   /* 0x58 */
void SetLineF4(void *prim) asm("func_80065074");   /* 0x4C */
void SetLineG4(void *prim) asm("func_80065094");   /* 0x5C */

/* Primitive attribute bits (bit 1 = semi-transparency, bit 0 = shade-texture). */
void SetSemiTrans(void *prim, s32 enabled) asm("func_80064E90");
void SetShadeTex(void *prim, s32 enabled) asm("func_80064EB8");

/* Ordering-table / primitive-list plumbing (24-bit "next" pointer in the tag). */
void SetPrimAddr(u32 *prim, u32 addr) asm("func_80064E54");
void TermPrim(u32 *prim) asm("func_80064E78");
s32 GetPrimAddr(u32 *prim) asm("func_80064DA4");
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void AddPrims(u32 *ot, u32 first, u32 *last) asm("func_80064E18");
void *ClearOTag(u32 *ot, s32 count) asm("func_80065CB0");
void *ClearOTagR(u32 *ot, s32 count) asm("func_80065D68");

/* Draw/display environment and texture-page helpers. */
/* CheckPrim (func_80065968) is declared locally per TU: callers pass either a
 * Rect * or a GpuRectPacked *, and gcc 2.6.3 will not accept both against one
 * prototype. */
void ClearImage(void *rect, u32 r, u32 g, u32 b) asm("func_80065A90");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
s32 MoveImage(GpuRectPacked *rect, u32 x, u32 y) asm("func_80065BEC");
void DrawSync(s32 mode) asm("func_800658FC");
u32 DrawSyncCallback(u32 callback) asm("func_80065804");
void DumpClut(s32 clut) asm("func_80064D64");
void DumpTPage(s32 tpage) asm("func_80064C94");
void DumpDrawEnv(DrawEnv *env) asm("func_80065234");
void DumpDispEnv(DispEnv *env) asm("func_800653B4");
s32 GetClut(s32 x, s32 y) asm("func_80064C7C");
s32 GetTPage(s32 tp, s32 abr, s32 x, s32 y) asm("func_80064BB4");
s32 GetDispEnv(s32 env) asm("func_8006655C");
s32 GetDrawEnv(s32 env) asm("func_80066074");
/* Draws one primitive immediately (DrawSync + push prim[4..] for prim[3]
 * words). The real PutDispEnv is func_800660AC. */
void DrawPrim(u8 *prim) asm("func_80065E00");
u8 *SetDefDrawEnv(u8 *env, s32 x, s32 y, s32 w, s32 h) asm("func_80064B78");
void SetDrawTPage(u8 *prim, s32 dfe, s32 dtd, s32 tpage) asm("func_800650E4");
void SetTexWindow(DrawPacket *prim, void *tw) asm("func_800665C8");
void SetDrawArea(DrawPacket *prim, Rect *rect) asm("func_80066604");
void SetDrawOffset(DrawPacket *prim, s16 *ofs) asm("func_80066688");
void SetDrawMode(
    DrawPacket *prim,
    s32 dfe,
    s32 dtd,
    u16 tpage,
    void *tw) asm("func_800666F4");
s32 LoadClut(void *clut, s32 x, s32 y) asm("func_80064A40");
s32 LoadClut2(void *clut, s32 x, s32 y) asm("func_800649D8");
/* D_800941E8 (mode) and D_800941EA (debug level) accessors. */
s32 GetGraphType(void) asm("func_800657E4");
s32 GetGraphDebug(void) asm("func_800657F4");
/* GP1(03h) display enable: 0 blanks the screen (and clears the cached
 * DISPENV), non-zero shows it. Named from its own "SetDispMask(%d)..." trace
 * string at D_80013520. */
void SetDispMask(s32 mask) asm("func_80065860");

/* libgpu-internal byte fill helper. */
void MemFill(u8 *dst, u8 value, s32 count) asm("func_80068180");

#endif
