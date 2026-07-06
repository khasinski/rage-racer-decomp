#include "common.h"

void func_80064EE0(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x20;
}

void func_80064EF4(u8 *arg0) {
    arg0[3] = 7;
    arg0[7] = 0x24;
}

void func_80064F08(u8 *arg0) {
    arg0[3] = 6;
    arg0[7] = 0x30;
}

void func_80064F1C(u8 *arg0) {
    arg0[3] = 9;
    arg0[7] = 0x34;
}

void func_80064F30(u8 *arg0) {
    arg0[3] = 5;
    arg0[7] = 0x28;
}

void func_80064F44(u8 *arg0) {
    arg0[3] = 9;
    arg0[7] = 0x2C;
}

void func_80064F58(u8 *arg0) {
    arg0[3] = 8;
    arg0[7] = 0x38;
}

void func_80064F6C(u8 *arg0) {
    arg0[3] = 0xC;
    arg0[7] = 0x3C;
}

void func_80064F80(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x74;
}

void func_80064F94(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x7C;
}

void func_80064FA8(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x64;
}

void func_80064FBC(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x68;
}

void func_80064FD0(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x70;
}

void func_80064FE4(u8 *arg0) {
    arg0[3] = 2;
    arg0[7] = 0x78;
}

void func_80064FF8(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x60;
}

void func_8006500C(u8 *arg0) {
    arg0[3] = 3;
    arg0[7] = 0x40;
}

void func_80065020(u8 *arg0) {
    arg0[3] = 4;
    arg0[7] = 0x50;
}

void func_80065034(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 5;
    arg0[7] = 0x48;
    *(u32 *)&arg0[0x14] = value;
}

void func_80065054(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 7;
    arg0[7] = 0x58;
    *(u32 *)&arg0[0x1C] = value;
}

void func_80065074(u8 *arg0) {
    u32 value;

    value = 0x55555555;
    arg0[3] = 6;
    arg0[7] = 0x4C;
    *(u32 *)&arg0[0x18] = value;
}

void func_80065094(u8 *arg0) {
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

void func_800650C8(u8 *arg0) {
    arg0[3] = 5;
    arg0[7] = 1;
    *(u32 *)&arg0[8] = 0x80000000;
}
