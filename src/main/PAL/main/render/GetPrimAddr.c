#include "common.h"

typedef struct PrimTag {
    unsigned addr : 24;
    unsigned len : 8;
} PrimTag;

s32 GetPrimAddr(u32 *p) {
    return ((PrimTag *)p)->addr | 0x80000000;
}

s32 IsEndPrim(u32 *p) {
    return ((PrimTag *)p)->addr == 0x00FFFFFF;
}

void AddPrim(u32 *ot, u32 *p) {
    PrimTag *otTag = (PrimTag *)ot;
    PrimTag *primTag = (PrimTag *)p;

    primTag->addr = otTag->addr;
    otTag->addr = (u32)primTag;
}

void AddPrims(u32 *ot, u32 first, u32 *last) {
    PrimTag *otTag = (PrimTag *)ot;
    PrimTag *firstTag = (PrimTag *)first;
    PrimTag *lastTag = (PrimTag *)last;

    lastTag->addr = otTag->addr;
    otTag->addr = (u32)firstTag;
}

void SetPrimAddr(u32 *p, u32 addr) {
    ((PrimTag *)p)->addr = addr;
}

void TermPrim(u32 *p) {
    ((PrimTag *)p)->addr = 0x00FFFFFF;
}

void SetSemiTrans(u8 *p, s32 abe) {
    s32 value;

    if (abe != 0) {
        value = p[7] | 2;
    } else {
        value = p[7] & 0xFD;
    }
    p[7] = value;
}

void SetShadeTex(u8 *p, s32 tge) {
    s32 value;

    if (tge != 0) {
        value = p[7] | 1;
    } else {
        value = p[7] & 0xFE;
    }
    p[7] = value;
}

void SetPolyF3(u8 *p) {
    p[3] = 4;
    p[7] = 0x20;
}

void SetPolyFT3(u8 *p) {
    p[3] = 7;
    p[7] = 0x24;
}

void SetPolyG3(u8 *p) {
    p[3] = 6;
    p[7] = 0x30;
}

void SetPolyGT3(u8 *p) {
    p[3] = 9;
    p[7] = 0x34;
}

void SetPolyF4(u8 *p) {
    p[3] = 5;
    p[7] = 0x28;
}

void SetPolyFT4(u8 *p) {
    p[3] = 9;
    p[7] = 0x2C;
}

void SetPolyG4(u8 *p) {
    p[3] = 8;
    p[7] = 0x38;
}

void SetPolyGT4(u8 *p) {
    p[3] = 0xC;
    p[7] = 0x3C;
}

void SetSprt8(u8 *p) {
    p[3] = 3;
    p[7] = 0x74;
}

void SetSprt16(u8 *p) {
    p[3] = 3;
    p[7] = 0x7C;
}

void SetSprt(u8 *p) {
    p[3] = 4;
    p[7] = 0x64;
}

void SetTile1(u8 *p) {
    p[3] = 2;
    p[7] = 0x68;
}

void SetTile8(u8 *p) {
    p[3] = 2;
    p[7] = 0x70;
}

void SetTile16(u8 *p) {
    p[3] = 2;
    p[7] = 0x78;
}

void SetTile(u8 *p) {
    p[3] = 3;
    p[7] = 0x60;
}

void SetLineF2(u8 *p) {
    p[3] = 3;
    p[7] = 0x40;
}

void SetLineG2(u8 *p) {
    p[3] = 4;
    p[7] = 0x50;
}

void SetLineF3(u8 *p) {
    u32 value;

    value = 0x55555555;
    p[3] = 5;
    p[7] = 0x48;
    *(u32 *)&p[0x14] = value;
}

void SetLineG3(u8 *p) {
    u32 value;

    value = 0x55555555;
    p[3] = 7;
    p[7] = 0x58;
    *(u32 *)&p[0x1C] = value;
}

void SetLineF4(u8 *p) {
    u32 value;

    value = 0x55555555;
    p[3] = 6;
    p[7] = 0x4C;
    *(u32 *)&p[0x18] = value;
}

void SetLineG4(u8 *p) {
    u32 value;

    value = 0x55555555;
    p[3] = 9;
    p[7] = 0x5C;
    *(u32 *)&p[0x24] = value;
}

void SetDrawPacketTag(u8 *p) {
    p[3] = 3;
    p[7] = 2;
}

void SetDrawMove(u8 *p) {
    p[3] = 5;
    p[7] = 1;
    *(u32 *)&p[8] = 0x80000000;
}
