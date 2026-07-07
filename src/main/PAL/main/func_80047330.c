#include "common.h"

void func_80065020(u8 *arg0);
void SetSemiTrans(u8 *arg0, s32 enabled) asm("func_80064E90");
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_80017390(void *ot, void *prim, s32 arg2);

void func_80047330(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r0, u8 g0, u8 b0, u8 r1, u8 g1, u8 b1, u8 alpha) {
    register s32 x0Reg asm("$17");
    register s32 y0Reg asm("$18");
    register s32 x1Reg asm("$19");
    register s32 y1Reg asm("$20");
    register s32 r0Reg asm("$21");
    register s32 g0Reg asm("$22");
    register s32 b0Reg asm("$23");
    register s32 alphaReg asm("$16");
    register u8 *a0Reg asm("$4");
    u8 *prim;
    u8 *oldPrim;
    u8 r1Local;
    u8 g1Local;
    u8 b1Local;

    prim = *(u8 **)0x1F800000;
    y1Reg = y1;
    r0Reg = r0;
    g0Reg = g0;
    b0Reg = b0;
    asm("" : : "r"(y1Reg), "r"(r0Reg), "r"(g0Reg), "r"(b0Reg));
    r1Local = r1;
    alphaReg = alpha;
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;
    g1Local = g1;
    b1Local = b1;

    func_80065020(prim);
    a0Reg = prim;
    asm("andi $16,$16,0xff" : : "r"(a0Reg));
    SetSemiTrans(a0Reg, alphaReg != 0xFF);

    *(s16 *)(prim + 0x8) = x0Reg;
    *(s16 *)(prim + 0xA) = y0Reg;
    *(s16 *)(prim + 0x10) = x1Reg;
    *(s16 *)(prim + 0x12) = y1Reg;
    prim[0x4] = r0Reg;
    prim[0x5] = g0Reg;
    prim[0x6] = b0Reg;
    prim[0xC] = r1Local;
    prim[0xD] = g1Local;
    prim[0xE] = b1Local;

    oldPrim = prim;
    prim += 0x14;
    AddPrim(ot, oldPrim);

    if (alphaReg != 0xFF) {
        prim = func_80017390(ot, prim, alphaReg);
    }

    *(u8 **)0x1F800000 = prim;
}
