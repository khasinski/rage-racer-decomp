#include "common.h"

void func_80064EE0(u8 *arg0);
void SetSemiTrans(u8 *arg0, s32 enabled) asm("func_80064E90");
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_80017390(void *ot, void *prim, s32 arg2);

void func_80046BA0(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u8 r, u8 g, u8 b, s32 semiTrans, u32 arg11) {
    register s32 semiReg asm("$17");
    register u32 flagsReg asm("$16");
    register s32 y1Reg asm("$18");
    register s32 x2Reg asm("$19");
    register s32 y2Reg asm("$20");
    register s32 rReg asm("$21");
    register s32 gReg asm("$22");
    register s32 bReg asm("$23");
    u8 *prim;
    u8 *oldPrim;

    prim = *(u8 **)0x1F800000;
    semiReg = semiTrans;
    flagsReg = arg11;
    y1Reg = y1;
    x2Reg = x2;
    y2Reg = y2;
    rReg = r;
    gReg = g;
    bReg = b;

    func_80064EE0(prim);
    SetSemiTrans(prim, semiReg);

    *(s16 *)(prim + 0x8) = x0;
    *(s16 *)(prim + 0xA) = y0;
    *(s16 *)(prim + 0xC) = x1;
    *(s16 *)(prim + 0xE) = y1Reg;
    *(s16 *)(prim + 0x10) = x2Reg;
    *(s16 *)(prim + 0x12) = y2Reg;
    prim[0x4] = rReg;
    prim[0x5] = gReg;
    prim[0x6] = bReg;

    oldPrim = prim;
    prim += 0x14;
    AddPrim(ot, oldPrim);

    semiReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = func_80017390(ot, prim, semiReg & 0xFFFF);
    }

    *(u8 **)0x1F800000 = prim;
}
