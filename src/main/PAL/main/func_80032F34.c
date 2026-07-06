#include "common.h"

void func_80064FF8(u8 *prim);
void func_80064DDC(void *ot, void *prim);

u8 *func_80032F34(void *ot, u8 *prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b) {
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
    func_80064DDC(ot, oldPrim);
    return prim;
}
