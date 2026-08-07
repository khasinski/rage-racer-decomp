#include "common.h"

typedef struct PrimTag {
    unsigned addr : 24;
    unsigned len : 8;
} PrimTag;

s32 GetPrimAddr(u32 *arg0) {
    return (*arg0 & 0x00FFFFFF) | 0x80000000;
}

s32 IsEndPrim(u32 *arg0) {
    return ((*arg0 & 0x00FFFFFF) == 0x00FFFFFF);
}

void AddPrim(u32 *arg0, u32 *arg1) {
    PrimTag *ot = (PrimTag *)arg0;
    PrimTag *prim = (PrimTag *)arg1;

    prim->addr = ot->addr;
    ot->addr = (u32)prim;
}

void AddPrims(u32 *arg0, u32 arg1, u32 *arg2) {
    PrimTag *ot = (PrimTag *)arg0;
    PrimTag *first = (PrimTag *)arg1;
    PrimTag *last = (PrimTag *)arg2;

    last->addr = ot->addr;
    ot->addr = (u32)first;
}

void SetPrimAddr(u32 *arg0, u32 arg1) {
    *arg0 = (*arg0 & 0xFF000000) | (arg1 & 0x00FFFFFF);
}

void TermPrim(u32 *arg0) {
    *arg0 |= 0x00FFFFFF;
}

void SetSemiTrans(u8 *arg0, s32 arg1) {
    s32 value;

    if (arg1 != 0) {
        value = arg0[7] | 2;
    } else {
        value = arg0[7] & 0xFD;
    }
    arg0[7] = value;
}

void SetShadeTex(u8 *arg0, s32 arg1) {
    s32 value;

    if (arg1 != 0) {
        value = arg0[7] | 1;
    } else {
        value = arg0[7] & 0xFE;
    }
    arg0[7] = value;
}

void SetPolyF3(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x20;
}

void SetPolyFT3(u8 *arg0) {
    arg0[3] = 7;
    arg0[7] = 0x24;
}

void SetPolyG3(u8 *arg0) {
    arg0[3] = 6;
    arg0[7] = 0x30;
}

void SetPolyGT3(u8 *arg0) {
    arg0[3] = 9;
    arg0[7] = 0x34;
}

void SetPolyF4(u8 *arg0) {
    arg0[3] = 5;
    arg0[7] = 0x28;
}

void SetPolyFT4(u8 *arg0) {
    arg0[3] = 9;
    arg0[7] = 0x2C;
}

void SetPolyG4(u8 *arg0) {
    arg0[3] = 8;
    arg0[7] = 0x38;
}

void SetPolyGT4(u8 *arg0) {
    arg0[3] = 0xC;
    arg0[7] = 0x3C;
}

void SetSprt8(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x74;
}

void SetSprt16(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x7C;
}

void SetSprt(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x64;
}

void SetTile1(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x68;
}

void SetTile8(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x70;
}

void SetTile16(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x78;
}

void SetTile(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x60;
}

void SetLineF2(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x40;
}

void SetLineG2(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x50;
}

void SetLineF3(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 5;
    arg0[7] = 0x48;
    *(u32 *)&arg0[0x14] = value;
}

void SetLineG3(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 7;
    arg0[7] = 0x58;
    *(u32 *)&arg0[0x1C] = value;
}

void SetLineF4(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 6;
    arg0[7] = 0x4C;
    *(u32 *)&arg0[0x18] = value;
}

void SetLineG4(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 9;
    arg0[7] = 0x5C;
    *(u32 *)&arg0[0x24] = value;
}

void SetDrawPacketTag(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 2;
}

void SetDrawMove(u8 *arg0) {
    arg0[3] = 5;
    arg0[7] = 1;
    *(u32 *)&arg0[8] = 0x80000000;
}
