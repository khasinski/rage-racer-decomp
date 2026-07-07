#include "common.h"

void func_80064FF8(u8 *arg0);
void SetSemiTrans(u8 *arg0, s32 enabled) asm("func_80064E90");
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_80017390(void *ot, void *prim, s32 arg2);

void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) {
    register s32 x0Reg asm("$17");
    register s32 y0Reg asm("$18");
    register s32 x1Reg asm("$19");
    register s32 y1Reg asm("$20");
    register s32 rReg asm("$21");
    register s32 gReg asm("$22");
    register s32 bReg asm("$23");
    register s32 alphaReg asm("$16");
    register u8 *a0Reg asm("$4");
    u8 *prim;
    u8 *oldPrim;

    prim = *(u8 **)0x1F800000;
    y1Reg = y1;
    rReg = r;
    gReg = g;
    bReg = b;
    alphaReg = alpha;
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;

    func_80064FF8(prim);
    a0Reg = prim;
    asm("andi $16,$16,0xff" : : "r"(a0Reg));
    SetSemiTrans(a0Reg, alphaReg != 0xFF);

    *(s16 *)(prim + 0x8) = x0Reg;
    *(s16 *)(prim + 0xA) = y0Reg;
    *(s16 *)(prim + 0xC) = x1Reg;
    *(s16 *)(prim + 0xE) = y1Reg;
    prim[0x4] = rReg;
    prim[0x5] = gReg;
    prim[0x6] = bReg;

    oldPrim = prim;
    prim += 0x10;
    AddPrim(ot, oldPrim);

    if (alphaReg != 0xFF) {
        prim = func_80017390(ot, prim, alphaReg);
    }

    *(u8 **)0x1F800000 = prim;
}
