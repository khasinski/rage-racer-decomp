#include "common.h"

void func_80064FF8(u8 *prim);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");

/*
 * Packs a TILE (SetTile + AddPrim) at `prim`, links it into `ot` and returns
 * the cursor advanced past the 0x10-byte packet. Declared per translation unit
 * rather than in a header: callers disagree on whether `ot`/`prim` are pointers
 * or s32, and gcc 2.6.3 will not take both against one prototype.
 */
u8 *GameAddTilePrim(void *ot, u8 *prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b) asm("func_80032F34");
u8 *GameAddTilePrim(void *ot, u8 *prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b) {
    u8 *oldPrim;

    func_80064FF8(prim);

    oldPrim = prim;
    *(s16 *)(prim + 0x08) = x;
    *(s16 *)(prim + 0x0A) = y;
    *(s16 *)(prim + 0x0C) = w;
    *(s16 *)(prim + 0x0E) = h;
    prim[0x04] = r;
    prim[0x05] = g;
    prim[0x06] = b;

    prim += 0x10;
    AddPrim(ot, oldPrim);
    return prim;
}
