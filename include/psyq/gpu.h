#ifndef RAGE_PC_PSYQ_GPU_H
#define RAGE_PC_PSYQ_GPU_H

#include <sys/types.h>

#include "common.h"

typedef struct Rect {
    short x;
    short y;
    short w;
    short h;
} Rect;

typedef struct GpuRectPacked {
    u_long xy;
    short w;
    short h;
} GpuRectPacked;

typedef struct GpuTexWindow {
    u_char x;
    u_char pad1;
    u_char y;
    u_char pad3;
    short w;
    short h;
} GpuTexWindow;

typedef struct DispEnv {
    Rect disp;
    Rect screen;
    u_char isinter;
    u_char isrgb24;
} DispEnv;

typedef struct DrawEnv {
    Rect clip;
    short ofs[2];
    Rect tw;
    u_short tpage;
    u_char dtd;
    u_char dfe;
} DrawEnv;

/*
 * The libgpu primitive packets the game builds, in PSY-Q layout: a 4-byte
 * ordering-table tag that AddPrim links, then the packed command word (rgb of
 * vertex 0 + the primitive code the Set* helpers stamp in).
 */
/* One deferred GPU operation in g_GpuQueue: a callback, its two arguments and
 * the packet it was handed. */
typedef struct QEntry {
    void (*cb)(long, long);
    long arg;
    long tag;
    long params[21];
} QEntry;

typedef struct P_TAG {
    u_long tag;
    u_char r0;
    u_char g0;
    u_char b0;
    u_char code;
} P_TAG;

/* Flat triangle, 0x14 bytes. func_80064EE0 = SetPolyF3. */
typedef struct POLY_F3 {
    P_TAG t;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
} POLY_F3;

/* Flat quad, 0x18 bytes. func_80064F30 = SetPolyF4. */
typedef struct POLY_F4 {
    P_TAG t;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    short x3;
    short y3;
} POLY_F4;

/* Textured quad, 0x28 bytes. func_80064F44 = SetPolyFT4. */
typedef struct POLY_FT4 {
    P_TAG t;
    short x0;
    short y0;
    u_char u0;
    u_char v0;
    u_short clut;
    short x1;
    short y1;
    u_char u1;
    u_char v1;
    u_short tpage;
    short x2;
    short y2;
    u_char u2;
    u_char v2;
    u_short pad1E;
    short x3;
    short y3;
    u_char u3;
    u_char v3;
    u_short pad26;
} POLY_FT4;

/* Textured sprite, 0x14 bytes. func_80064FA8 = SetSprt. */
typedef struct SPRT {
    P_TAG t;
    short x0;
    short y0;
    u_char u0;
    u_char v0;
    u_short clut;
    short w;
    short h;
} SPRT;

/* Solid rectangle, 0x10 bytes. func_80064FF8 = SetTile. */
typedef struct TILE {
    P_TAG t;
    short x0;
    short y0;
    short w;
    short h;
} TILE;

/* Flat line, 0x10 bytes. func_8006500C = SetLineF2. */
typedef struct LINE_F2 {
    P_TAG t;
    short x0;
    short y0;
    short x1;
    short y1;
} LINE_F2;

/* Flat 3-point polyline, 0x18 bytes. func_80065034 = SetLineF3. */
typedef struct LINE_F3 {
    P_TAG t;
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    u_long pad14;
} LINE_F3;

/* Gradient line, 0x14 bytes. func_80065020 = SetLineG2. */
typedef struct LINE_G2 {
    P_TAG t;
    short x0;
    short y0;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char pad0F;
    short x1;
    short y1;
} LINE_G2;

typedef struct DrawPacket {
    u_char pad0[3];
    u_char code;
    u_long x0y0;
    u_long x1y1;
} DrawPacket;

/*
 * The libgpu driver table at 0x800941A0 (D_800941E0 points at it), dumped in
 * asm/PAL/main/data/main/6BE64.data.s. Slots holding a `u_long` are worker
 * function addresses passed to `send` rather than called directly:
 *   +0x04 func_8006767C  +0x08 Gpu_AddQueue      +0x0C Gpu_ClearImage
 *   +0x10 Gpu_WriteGp1   +0x14 Gpu_WriteGp0Words +0x18 Gpu_StartDmaTransfer
 *   +0x1C Gpu_StoreImage +0x20 Gpu_LoadImage     +0x24 Gpu_ExecuteQueue
 *   +0x28 Gpu_GetControlMirrorByte               +0x2C Gpu_ClearOTagDma
 *   +0x30 _param         +0x34 Gpu_Reset         +0x38 _status
 *   +0x3C Gpu_DrawSync
 */
typedef struct GpuCallbacks {
    u_char pad0[0x8];
    long (*send)(u_long worker, void *buf, long size, u_long data);
    u_long cmd0C;
    void (*submit)(long cmd);
    void (*writeGp0Words)(void *src, long count);
    u_long sendList;
    u_long storeImage;
    u_long loadImage;
    u_char pad24[0x28 - 0x24];
    long (*read)(long cmd);
    void (*clearOTag)(void *ot, long count);
    u_char pad30[0x34 - 0x30];
    void (*resetGraph)(long mode);
    long (*status)(void);
    void (*drawSync)(long mode);
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
void SetSemiTrans(void *prim, long enabled) asm("func_80064E90");
void SetShadeTex(void *prim, long enabled) asm("func_80064EB8");

/* Ordering-table / primitive-list plumbing (24-bit "next" pointer in the tag). */
void SetPrimAddr(u_long *prim, u_long addr) asm("func_80064E54");
void TermPrim(u_long *prim) asm("func_80064E78");
long GetPrimAddr(u_long *prim) asm("func_80064DA4");
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void AddPrims(u_long *ot, u_long first, u_long *last) asm("func_80064E18");
void *ClearOTag(u_long *ot, long count) asm("func_80065CB0");
void *ClearOTagR(u_long *ot, long count) asm("func_80065D68");

/* Draw/display environment and texture-page helpers. */
/* CheckPrim (func_80065968) is declared locally per TU: callers pass either a
 * Rect * or a GpuRectPacked *, and gcc 2.6.3 will not accept both against one
 * prototype. */
void ClearImage(void *rect, u_char r, u_char g, u_char b) asm("func_80065A90");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
long MoveImage(GpuRectPacked *rect, u_long x, u_long y) asm("func_80065BEC");
/* LibRef47 6-33 returns long (queue length for mode 1); no caller here uses the
 * result, so it is declared void. */
void DrawSync(long mode) asm("func_800658FC");
u_long DrawSyncCallback(u_long callback) asm("func_80065804");
void DumpClut(long clut) asm("func_80064D64");
void DumpTPage(long tpage) asm("func_80064C94");
void DumpDrawEnv(DrawEnv *env) asm("func_80065234");
void DumpDispEnv(DispEnv *env) asm("func_800653B4");
long GetClut(long x, long y) asm("func_80064C7C");
long GetTPage(long tp, long abr, long x, long y) asm("func_80064BB4");
long GetDispEnv(long env) asm("func_8006655C");
long GetDrawEnv(long env) asm("func_80066074");
/* Draws one primitive immediately (DrawSync + push prim[4..] for prim[3]
 * words). The real PutDispEnv is func_800660AC. */
void DrawPrim(u_char *prim) asm("func_80065E00");
/* Fills the 0x1C-byte DRAWENV head: clip, ofs, tw, tpage, dtd, dfe, isbg, rgb.
 * `dfe` comes from the buffer height and the DMA interrupt state. */
void *SetDefDrawEnv(u_char *env, long x, long y, long w, long h) asm("func_80064AA8");
/* Fills the 0x14-byte DISPENV: disp Rect, screen Rect, isinter, isrgb24.
 * Was bound to SetDefDrawEnv here; that was wrong (see docs/names.md 17). */
u_char *SetDefDispEnv(u_char *env, long x, long y, long w, long h) asm("func_80064B78");
void SetDrawTPage(u_char *prim, long dfe, long dtd, long tpage) asm("func_800650E4");
void SetTexWindow(DrawPacket *prim, void *tw) asm("func_800665C8");
void SetDrawArea(DrawPacket *prim, Rect *rect) asm("func_80066604");
void SetDrawOffset(DrawPacket *prim, short *ofs) asm("func_80066688");
void SetDrawMode(
    DrawPacket *prim,
    long dfe,
    long dtd,
    u_short tpage,
    void *tw) asm("func_800666F4");
long LoadClut(void *clut, long x, long y) asm("func_80064A40");
long LoadClut2(void *clut, long x, long y) asm("func_800649D8");
/* D_800941E8 (mode) and D_800941EA (debug level) accessors. */
long GetGraphType(void) asm("func_800657E4");
long GetGraphDebug(void) asm("func_800657F4");
/* GP1(03h) display enable: 0 blanks the screen (and clears the cached
 * DISPENV), non-zero shows it. Named from its own "SetDispMask(%d)..." trace
 * string at D_80013520. */
void SetDispMask(long mask) asm("func_80065860");

/* libgpu-internal byte fill helper. */
void MemFill(u_char *dst, u_char value, long count) asm("func_80068180");

#endif
