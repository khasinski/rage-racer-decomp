#include "common.h"
#include "psyq/gpu.h"

s32 GetPrimAddr(u32 *prim) {
    return ((P_TAG *)prim)->addr | 0x80000000;
}

s32 IsEndPrim(u32 *prim) {
    return ((P_TAG *)prim)->addr == 0x00FFFFFF;
}

void AddPrim(void *ot, void *prim) {
    P_TAG *otTag = (P_TAG *)ot;
    P_TAG *primTag = (P_TAG *)prim;

    primTag->addr = otTag->addr;
    otTag->addr = (u32)primTag;
}

void AddPrims(void *ot, void *first, void *last) {
    P_TAG *otTag = (P_TAG *)ot;
    P_TAG *firstTag = (P_TAG *)first;
    P_TAG *lastTag = (P_TAG *)last;

    lastTag->addr = otTag->addr;
    otTag->addr = (u32)firstTag;
}

void SetPrimAddr(u32 *prim, u32 addr) {
    ((P_TAG *)prim)->addr = addr;
}

void TermPrim(u32 *prim) {
    ((P_TAG *)prim)->addr = 0x00FFFFFF;
}

void SetSemiTrans(void *prim, s32 abe) {
    u8 *p = prim;
    s32 value;

    if (abe != 0) {
        value = p[7] | 2;
    } else {
        value = p[7] & 0xFD;
    }
    p[7] = value;
}

void SetShadeTex(void *prim, s32 tge) {
    u8 *p = prim;
    s32 value;

    if (tge != 0) {
        value = p[7] | 1;
    } else {
        value = p[7] & 0xFE;
    }
    p[7] = value;
}

void SetPolyF3(void *prim) {
    u8 *p = prim;

    p[3] = 4;
    p[7] = 0x20;
}

void SetPolyFT3(void *prim) {
    u8 *p = prim;

    p[3] = 7;
    p[7] = 0x24;
}

void SetPolyG3(void *prim) {
    u8 *p = prim;

    p[3] = 6;
    p[7] = 0x30;
}

void SetPolyGT3(void *prim) {
    u8 *p = prim;

    p[3] = 9;
    p[7] = 0x34;
}

void SetPolyF4(void *prim) {
    u8 *p = prim;

    p[3] = 5;
    p[7] = 0x28;
}

void SetPolyFT4(void *prim) {
    u8 *p = prim;

    p[3] = 9;
    p[7] = 0x2C;
}

void SetPolyG4(void *prim) {
    u8 *p = prim;

    p[3] = 8;
    p[7] = 0x38;
}

void SetPolyGT4(void *prim) {
    u8 *p = prim;

    p[3] = 0xC;
    p[7] = 0x3C;
}

void SetSprt8(void *prim) {
    u8 *p = prim;

    p[3] = 3;
    p[7] = 0x74;
}

void SetSprt16(void *prim) {
    u8 *p = prim;

    p[3] = 3;
    p[7] = 0x7C;
}

void SetSprt(void *prim) {
    u8 *p = prim;

    p[3] = 4;
    p[7] = 0x64;
}

void SetTile1(void *prim) {
    u8 *p = prim;

    p[3] = 2;
    p[7] = 0x68;
}

void SetTile8(void *prim) {
    u8 *p = prim;

    p[3] = 2;
    p[7] = 0x70;
}

void SetTile16(void *prim) {
    u8 *p = prim;

    p[3] = 2;
    p[7] = 0x78;
}

void SetTile(void *prim) {
    u8 *p = prim;

    p[3] = 3;
    p[7] = 0x60;
}

void SetLineF2(void *prim) {
    u8 *p = prim;

    p[3] = 3;
    p[7] = 0x40;
}

void SetLineG2(void *prim) {
    u8 *p = prim;

    p[3] = 4;
    p[7] = 0x50;
}

void SetLineF3(void *prim) {
    u8 *p = prim;

    u32 value;

    value = 0x55555555;
    p[3] = 5;
    p[7] = 0x48;
    *(u32 *)&p[0x14] = value;
}

void SetLineG3(void *prim) {
    u8 *p = prim;

    u32 value;

    value = 0x55555555;
    p[3] = 7;
    p[7] = 0x58;
    *(u32 *)&p[0x1C] = value;
}

void SetLineF4(void *prim) {
    u8 *p = prim;

    u32 value;

    value = 0x55555555;
    p[3] = 6;
    p[7] = 0x4C;
    *(u32 *)&p[0x18] = value;
}

void SetLineG4(void *prim) {
    u8 *p = prim;

    u32 value;

    value = 0x55555555;
    p[3] = 9;
    p[7] = 0x5C;
    *(u32 *)&p[0x24] = value;
}

void SetDrawPacketTag(void *prim) {
    u8 *p = prim;

    p[3] = 3;
    p[7] = 2;
}

void SetDrawMove(void *prim) {
    u8 *p = prim;

    p[3] = 5;
    p[7] = 1;
    *(u32 *)&p[8] = 0x80000000;
}
