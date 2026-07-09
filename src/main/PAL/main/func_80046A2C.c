#include "common.h"

void func_80064FA8(u8 *arg0);
void func_80064EB8(u8 *arg0, s32 enabled);
void func_80064E90(u8 *arg0, s32 enabled);
void func_80064DDC(void *ot, void *prim);
void *func_80017390(void *ot, void *prim, s32 arg2);

void func_80046A2C(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0, u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags) {
    u8 *prim;
    register s32 shadeReg asm("$18");
    register s32 semiReg asm("$19");
    register u32 flagsReg asm("$17");
    register s32 y1Reg asm("$20");
    register s32 u0Reg asm("$21");
    register s32 v0Reg asm("$22");
    register s32 rReg asm("$23");
    register s32 clutReg asm("$16");
    u8 *oldPrim;
    s16 x0Local;
    s16 y0Local;
    s16 x1Local;
    u8 gLocal;
    u8 bLocal;
    s32 div;
    s32 base;
    u32 magic;

    prim = *(u8 **)0x1F800000;
    shadeReg = shadeTex;
    semiReg = semiTrans;
    asm("" : : "r"(prim), "r"(shadeReg), "r"(semiReg));
    flagsReg = flags;
    y1Reg = y1;
    u0Reg = u0;
    v0Reg = v0;
    rReg = r;
    x0Local = x0;
    y0Local = y0;
    x1Local = x1;
    bLocal = b;
    /* Match note: keep stack color load after callee-save argument setup. */
    asm("" : : "r"(flagsReg), "r"(y1Reg), "r"(u0Reg), "r"(v0Reg), "r"(rReg));
    gLocal = g;
    asm("" : : "r"(gLocal));
    clutReg = clutX;

    func_80064FA8(prim);
    func_80064EB8(prim, shadeReg);
    func_80064E90(prim, semiReg);

    *(s16 *)(prim + 0x8) = x0Local;
    *(s16 *)(prim + 0xA) = y0Local;
    *(s16 *)(prim + 0x10) = x1Local;
    *(s16 *)(prim + 0x12) = y1Reg;
    prim[0xC] = u0Reg;
    prim[0xD] = v0Reg;
    prim[0x4] = rReg;
    prim[0x5] = gLocal;
    prim[0x6] = bLocal;

    asm("" : "=r"(clutReg) : "0"(clutReg));
    magic = 0xCCCCCCCD;
    asm("" : : "r"(magic));
    clutReg &= 0xFFFF;
    div = clutReg / 20U;
    base = (div + 0x1E0) << 6;
    *(s16 *)(prim + 0xE) = base + (clutReg - (div * 20));

    oldPrim = prim;
    prim += 0x14;
    func_80064DDC(ot, oldPrim);

    clutReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = func_80017390(ot, prim, clutReg & 0xFFFF);
    }

    *(u8 **)0x1F800000 = prim;
}
