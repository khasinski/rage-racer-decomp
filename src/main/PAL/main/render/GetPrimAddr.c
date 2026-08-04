#include "common.h"

s32 GetPrimAddr(u32 *arg0) asm("func_80064DA4");
s32 GetPrimAddr(u32 *arg0) {
    return (*arg0 & 0x00FFFFFF) | 0x80000000;
}

s32 IsEndPrim(u32 *arg0) asm("func_80064DC0");
s32 IsEndPrim(u32 *arg0) {
    return ((*arg0 & 0x00FFFFFF) == 0x00FFFFFF);
}

void AddPrim(u32 *arg0, u32 *arg1) asm("func_80064DDC");
void AddPrim(u32 *arg0, u32 *arg1) {
    register u32 mask asm("$6") = 0x00FFFFFF;
    u32 tag = 0xFF000000;

    *arg1 = (*arg1 & tag) | (*arg0 & mask);
    *arg0 = (*arg0 & tag) | ((u32)arg1 & mask);
}

void AddPrims(u32 *arg0, u32 arg1, u32 *arg2) asm("func_80064E18");
void AddPrims(u32 *arg0, u32 arg1, u32 *arg2) {
    register u32 mask asm("$7") = 0x00FFFFFF;
    u32 tag = 0xFF000000;

    *arg2 = (*arg2 & tag) | (*arg0 & mask);
    *arg0 = (*arg0 & tag) | (arg1 & mask);
}

void SetPrimAddr(u32 *arg0, u32 arg1) asm("func_80064E54");
void SetPrimAddr(u32 *arg0, u32 arg1) {
    *arg0 = (*arg0 & 0xFF000000) | (arg1 & 0x00FFFFFF);
}

void TermPrim(u32 *arg0) asm("func_80064E78");
void TermPrim(u32 *arg0) {
    *arg0 |= 0x00FFFFFF;
}

void SetSemiTrans(u8 *arg0, s32 arg1) asm("func_80064E90");
void SetSemiTrans(u8 *arg0, s32 arg1) {
    s32 value;

    if (arg1 != 0) {
        value = arg0[7] | 2;
    } else {
        value = arg0[7] & 0xFD;
    }
    arg0[7] = value;
}

void SetShadeTex(u8 *arg0, s32 arg1) asm("func_80064EB8");
void SetShadeTex(u8 *arg0, s32 arg1) {
    s32 value;

    if (arg1 != 0) {
        value = arg0[7] | 1;
    } else {
        value = arg0[7] & 0xFE;
    }
    arg0[7] = value;
}

void SetPolyF3(u8 *arg0) asm("func_80064EE0");
void SetPolyF3(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x20;
}

void SetPolyFT3(u8 *arg0) asm("func_80064EF4");
void SetPolyFT3(u8 *arg0) {
    arg0[3] = 7;
    arg0[7] = 0x24;
}

void SetPolyG3(u8 *arg0) asm("func_80064F08");
void SetPolyG3(u8 *arg0) {
    arg0[3] = 6;
    arg0[7] = 0x30;
}

void SetPolyGT3(u8 *arg0) asm("func_80064F1C");
void SetPolyGT3(u8 *arg0) {
    arg0[3] = 9;
    arg0[7] = 0x34;
}

void SetPolyF4(u8 *arg0) asm("func_80064F30");
void SetPolyF4(u8 *arg0) {
    arg0[3] = 5;
    arg0[7] = 0x28;
}

void SetPolyFT4(u8 *arg0) asm("func_80064F44");
void SetPolyFT4(u8 *arg0) {
    arg0[3] = 9;
    arg0[7] = 0x2C;
}

void SetPolyG4(u8 *arg0) asm("func_80064F58");
void SetPolyG4(u8 *arg0) {
    arg0[3] = 8;
    arg0[7] = 0x38;
}

void SetPolyGT4(u8 *arg0) asm("func_80064F6C");
void SetPolyGT4(u8 *arg0) {
    arg0[3] = 0xC;
    arg0[7] = 0x3C;
}

void SetSprt8(u8 *arg0) asm("func_80064F80");
void SetSprt8(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x74;
}

void SetSprt16(u8 *arg0) asm("func_80064F94");
void SetSprt16(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x7C;
}

void SetSprt(u8 *arg0) asm("func_80064FA8");
void SetSprt(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x64;
}

void SetTile1(u8 *arg0) asm("func_80064FBC");
void SetTile1(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x68;
}

void SetTile8(u8 *arg0) asm("func_80064FD0");
void SetTile8(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x70;
}

void SetTile16(u8 *arg0) asm("func_80064FE4");
void SetTile16(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x78;
}

void SetTile(u8 *arg0) asm("func_80064FF8");
void SetTile(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x60;
}

void SetLineF2(u8 *arg0) asm("func_8006500C");
void SetLineF2(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x40;
}

void SetLineG2(u8 *arg0) asm("func_80065020");
void SetLineG2(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x50;
}

void SetLineF3(u8 *arg0) asm("func_80065034");
void SetLineF3(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 5;
    arg0[7] = 0x48;
    *(u32 *)&arg0[0x14] = value;
}

void SetLineG3(u8 *arg0) asm("func_80065054");
void SetLineG3(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 7;
    arg0[7] = 0x58;
    *(u32 *)&arg0[0x1C] = value;
}

void SetLineF4(u8 *arg0) asm("func_80065074");
void SetLineF4(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 6;
    arg0[7] = 0x4C;
    *(u32 *)&arg0[0x18] = value;
}

void SetLineG4(u8 *arg0) asm("func_80065094");
void SetLineG4(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 9;
    arg0[7] = 0x5C;
    *(u32 *)&arg0[0x24] = value;
}

void func_800650B4(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 2;
}

void SetDrawMove(u8 *arg0) asm("func_800650C8");
void SetDrawMove(u8 *arg0) {
    arg0[3] = 5;
    arg0[7] = 1;
    *(u32 *)&arg0[8] = 0x80000000;
}
