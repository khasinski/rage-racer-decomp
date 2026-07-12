#include "common.h"

void func_80064F44(u8 *prim);
void func_80064EB8(u8 *prim, s32 arg1);
void func_80064E90(u8 *prim, s32 arg1);
void func_80064DDC(s32 ot, u8 *prim);

void func_80046E00(s32 p1, s16 p2, s16 p3, s16 p4,
                   u16 p5, u16 p6, u16 p7, u16 p8, u16 p9,
                   u8 p10, u8 p11, u8 p12, u8 p13, u8 p14, u8 p15,
                   u8 p16, u8 p17, u8 p18, u8 p19, u8 p20,
                   u16 depth, s32 a1a, s32 a1b, u16 p24) {
    u8 *prim = *(u8 **)0x1F800000;
    u32 d;
    u32 otz;
    u32 rem;
    s32 otval;
    u8 *oldPrim;

    func_80064F44(prim);
    func_80064EB8(prim, a1a);
    func_80064E90(prim, a1b);
    *(u16 *)(prim + 0x8) = p2;
    *(u16 *)(prim + 0xA) = p3;
    *(u16 *)(prim + 0x10) = p4;
    *(u16 *)(prim + 0x12) = p5;
    *(u16 *)(prim + 0x18) = p6;
    *(u16 *)(prim + 0x1A) = p7;
    *(u16 *)(prim + 0x20) = p8;
    *(u16 *)(prim + 0x22) = p9;
    prim[0xC] = p10;
    prim[0xD] = p11;
    prim[0x14] = p12;
    prim[0x15] = p13;
    prim[0x1C] = p14;
    prim[0x1D] = p15;
    prim[0x24] = p16;
    prim[0x25] = p17;
    prim[0x4] = p18;
    prim[0x5] = p19;
    prim[0x6] = p20;
    *(u16 *)(prim + 0x16) = p24;
    d = depth;
    otz = d / 20;
    otval = (otz + 0x1E0) << 6;
    rem = d - otz * 20;
    otval = otval + rem;
    *(u16 *)(prim + 0xE) = otval;
    oldPrim = prim;
    prim += 0x28;
    func_80064DDC(p1, oldPrim);
    *(u8 **)0x1F800000 = prim;
}
