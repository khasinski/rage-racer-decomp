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

typedef struct DrawPacket {
    u8 pad0[3];
    u8 code;
    u32 x0y0;
    u32 x1y1;
} DrawPacket;

typedef struct GpuCallbacks {
    u8 pad0[0x8];
    s32 (*send)(u32 cmd, void *buf, s32 size, u32 data);
    u32 cmd0C;
    void (*submit)(s32 cmd);
    void (*putDispEnv)(void *env, s32 mode);
    u32 moveImage;
    u32 storeImage;
    u32 loadImage;
    u8 pad24[0x28 - 0x24];
    s32 (*read)(s32 cmd);
    void (*clearImage)(void *rect, s32 rgb);
    u8 pad30[0x34 - 0x30];
    void (*resetGraph)(s32 mode);
    s32 (*drawSyncStatus)(void);
    void (*drawSync)(s32 mode);
} GpuCallbacks;

/*
 * libgpu primitive initialisers. Each stamps the word count into prim[3] and
 * the GPU command byte into prim[7] (see the GP0 opcode in the comment).
 */
void SetPolyF3(u8 *prim) asm("func_80064EE0");   /* 0x20 */
void SetPolyFT3(u8 *prim) asm("func_80064EF4");  /* 0x24 */
void SetPolyG3(u8 *prim) asm("func_80064F08");   /* 0x30 */
void SetPolyGT3(u8 *prim) asm("func_80064F1C");  /* 0x34 */
void SetPolyF4(u8 *prim) asm("func_80064F30");   /* 0x28 */
void SetPolyFT4(u8 *prim) asm("func_80064F44");  /* 0x2C */
void SetPolyG4(u8 *prim) asm("func_80064F58");   /* 0x38 */
void SetPolyGT4(u8 *prim) asm("func_80064F6C");  /* 0x3C */
void SetSprt8(u8 *prim) asm("func_80064F80");    /* 0x74 */
void SetSprt16(u8 *prim) asm("func_80064F94");   /* 0x7C */
void SetSprt(u8 *prim) asm("func_80064FA8");     /* 0x64 */
void SetTile1(u8 *prim) asm("func_80064FBC");    /* 0x68 */
void SetTile8(u8 *prim) asm("func_80064FD0");    /* 0x70 */
void SetTile16(u8 *prim) asm("func_80064FE4");   /* 0x78 */
void SetTile(u8 *prim) asm("func_80064FF8");     /* 0x60 */
void SetLineF2(u8 *prim) asm("func_8006500C");   /* 0x40 */
void SetLineG2(u8 *prim) asm("func_80065020");   /* 0x50 */
void SetLineF3(u8 *prim) asm("func_80065034");   /* 0x48 */
void SetLineG3(u8 *prim) asm("func_80065054");   /* 0x58 */
void SetLineF4(u8 *prim) asm("func_80065074");   /* 0x4C */
void SetLineG4(u8 *prim) asm("func_80065094");   /* 0x5C */

/* Primitive attribute bits (bit 1 = semi-transparency, bit 0 = shade-texture). */
void SetSemiTrans(u8 *prim, s32 enabled) asm("func_80064E90");
void SetShadeTex(u8 *prim, s32 enabled) asm("func_80064EB8");

/* Ordering-table / primitive-list plumbing (24-bit "next" pointer in the tag). */
void SetPrimAddr(u32 *prim, u32 addr) asm("func_80064E54");
void TermPrim(u32 *prim) asm("func_80064E78");
s32 GetPrimAddr(u32 *prim) asm("func_80064DA4");
void AddPrim(u32 *ot, u32 *prim) asm("func_80064DDC");
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
void PutDispEnv(u8 *env) asm("func_80065E00");
u8 *SetDefDrawEnv(u8 *env, s32 x, s32 y, s32 w, s32 h) asm("func_80064B78");
void SetDrawTPage(u8 *prim, s32 dfe, s32 dtd, s32 tpage) asm("func_800650E4");
void SetTexWindow(u8 *prim, void *tw) asm("func_800665C8");
void SetDrawArea(DrawPacket *prim, Rect *rect) asm("func_80066604");
void SetDrawOffset(u8 *prim, s16 *ofs) asm("func_80066688");
void SetDrawMode(
    u8 *prim,
    s32 dfe,
    s32 dtd,
    u16 tpage,
    void *tw) asm("func_800666F4");
s32 LoadClut(void *clut, s32 x, s32 y) asm("func_80064A40");
s32 LoadClut2(void *clut, s32 x, s32 y) asm("func_800649D8");
/* D_800941E8 (mode) and D_800941EA (debug level) accessors. */
s32 GetGraphType(void) asm("func_800657E4");
s32 GetGraphDebug(void) asm("func_800657F4");

/* libgpu-internal byte fill helper. */
void MemFill(u8 *dst, u8 value, s32 count) asm("func_80068180");

#endif
