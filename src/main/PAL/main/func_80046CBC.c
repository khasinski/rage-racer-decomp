/* MASPSX_FLAGS: --rage-prolog-func-80046cbc */
#include "common.h"

void func_80064F30(u8 *arg0);
void func_80064E90(u8 *arg0, s32 enabled);
void func_80064DDC(void *ot, void *prim);
void *func_80017390(void *ot, void *prim, s32 arg2);

void func_80046CBC(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u8 r, u8 g, u8 b, s32 semiTrans, u32 flags) {
    u8 *prim;
    register s32 semiReg asm("$17");
    register u32 flagsReg asm("$16");
    register s32 y1Reg asm("$18");
    register s32 x2Reg asm("$19");
    register s32 y2Reg asm("$20");
    register s32 x3Reg asm("$21");
    register s32 y3Reg asm("$22");
    register s32 rReg asm("$23");
    u8 *oldPrim;
    s16 x0Local;
    s16 y0Local;
    s16 x1Local;
    u8 gLocal;
    u8 bLocal;

    prim = *(u8 **)0x1F800000;
    semiReg = semiTrans;
    flagsReg = flags;
    y1Reg = y1;
    x2Reg = x2;
    y2Reg = y2;
    x3Reg = x3;
    y3Reg = y3;
    rReg = r;
    x0Local = x0;
    y0Local = y0;
    x1Local = x1;
    gLocal = g;
    bLocal = b;

    func_80064F30(prim);
    func_80064E90(prim, semiReg);

    *(s16 *)(prim + 0x8) = x0Local;
    *(s16 *)(prim + 0xA) = y0Local;
    *(s16 *)(prim + 0xC) = x1Local;
    *(s16 *)(prim + 0xE) = y1Reg;
    *(s16 *)(prim + 0x10) = x2Reg;
    *(s16 *)(prim + 0x12) = y2Reg;
    *(s16 *)(prim + 0x14) = x3Reg;
    *(s16 *)(prim + 0x16) = y3Reg;
    prim[0x4] = rReg;
    prim[0x5] = gLocal;
    prim[0x6] = bLocal;

    oldPrim = prim;
    prim += 0x18;
    func_80064DDC(ot, oldPrim);

    semiReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = func_80017390(ot, prim, semiReg & 0xFFFF);
    }

    *(u8 **)0x1F800000 = prim;
}
