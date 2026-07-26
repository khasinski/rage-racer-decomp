#include "common.h"

void SetPolyFT4(u8 *prim) asm("func_80064F44");
void SetShadeTex(u8 *prim, s32 enabled) asm("func_80064EB8");
void SetSemiTrans(u8 *prim, s32 enabled) asm("func_80064E90");
void AddPrim(s32 ot, u8 *prim) asm("func_80064DDC");

/*
 * Packs a POLY_FT4 (textured quad) at the scratchpad cursor and links it into
 * the ordering table. clutIndex is a linear palette slot turned into VRAM clut
 * coordinates: 20 cluts per row starting at y = 0x1E0.
 */
void GameDrawTexturedQuad(s32 ot, s16 x0, s16 y0, s16 x1,
                   u16 y1, u16 x2, u16 y2, u16 x3, u16 y3,
                   u8 u0, u8 v0, u8 u1, u8 v1, u8 u2, u8 v2,
                   u8 u3, u8 v3, u8 r, u8 g, u8 b,
                   u16 clutIndex, s32 shadeTex, s32 semiTrans, u16 tpage) asm("func_80046E00");
void GameDrawTexturedQuad(s32 ot, s16 x0, s16 y0, s16 x1,
                   u16 y1, u16 x2, u16 y2, u16 x3, u16 y3,
                   u8 u0, u8 v0, u8 u1, u8 v1, u8 u2, u8 v2,
                   u8 u3, u8 v3, u8 r, u8 g, u8 b,
                   u16 clutIndex, s32 shadeTex, s32 semiTrans, u16 tpage) {
    u8 *prim = *(u8 **)0x1F800000;
    u32 d;
    u32 clutRow;
    u32 rem;
    s32 clut;
    u8 *oldPrim;

    SetPolyFT4(prim);
    SetShadeTex(prim, shadeTex);
    SetSemiTrans(prim, semiTrans);
    *(u16 *)(prim + 0x8) = x0;
    *(u16 *)(prim + 0xA) = y0;
    *(u16 *)(prim + 0x10) = x1;
    *(u16 *)(prim + 0x12) = y1;
    *(u16 *)(prim + 0x18) = x2;
    *(u16 *)(prim + 0x1A) = y2;
    *(u16 *)(prim + 0x20) = x3;
    *(u16 *)(prim + 0x22) = y3;
    prim[0xC] = u0;
    prim[0xD] = v0;
    prim[0x14] = u1;
    prim[0x15] = v1;
    prim[0x1C] = u2;
    prim[0x1D] = v2;
    prim[0x24] = u3;
    prim[0x25] = v3;
    prim[0x4] = r;
    prim[0x5] = g;
    prim[0x6] = b;
    *(u16 *)(prim + 0x16) = tpage;
    d = clutIndex;
    clutRow = d / 20;
    clut = (clutRow + 0x1E0) << 6;
    rem = d - clutRow * 20;
    clut = clut + rem;
    *(u16 *)(prim + 0xE) = clut;
    oldPrim = prim;
    prim += 0x28;
    AddPrim(ot, oldPrim);
    *(u8 **)0x1F800000 = prim;
}
