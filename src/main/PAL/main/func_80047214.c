#include "common.h"

void func_80065034(u8 *arg0);
void SetSemiTrans(u8 *arg0, s32 enabled) asm("func_80064E90");
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_80017390(void *ot, void *prim, s32 arg2);

void func_80047214(void *ot, s16 x0, s16 y0, s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b, u8 arg10) {
    u8 *prim;
    u8 *oldPrim;

    prim = *(u8 **)0x1F800000;
    func_80065034(prim);
    SetSemiTrans(prim, arg10 != 0xFF);

    *(s16 *)(prim + 0x8) = x0;
    *(s16 *)(prim + 0xA) = y0;
    *(s16 *)(prim + 0xC) = x1;
    *(s16 *)(prim + 0xE) = y1;
    *(s16 *)(prim + 0x10) = x2;
    *(s16 *)(prim + 0x12) = y2;
    prim[0x4] = r;
    prim[0x5] = g;
    prim[0x6] = b;

    oldPrim = prim;
    prim += 0x18;
    AddPrim(ot, oldPrim);

    if (arg10 != 0xFF) {
        prim = func_80017390(ot, prim, arg10);
    }

    *(u8 **)0x1F800000 = prim;
}
