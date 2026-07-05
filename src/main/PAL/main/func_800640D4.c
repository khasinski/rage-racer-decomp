#include "common.h"

extern volatile u32 *D_80083164;
extern volatile u32 *D_80083168;
extern volatile u32 *D_8008316C;
extern volatile u32 *D_80083170;
extern volatile u32 *D_80083174;
extern volatile u32 *D_80083178;
extern volatile u32 *D_80083194;
extern volatile u32 *D_80083198;
extern volatile u32 *D_8008319C;
extern u8 D_800132C8[];
extern u8 D_8008305C[];
extern u8 D_800830E0[];

s32 func_800642F4(void);
s32 func_8006438C(void);
void func_8001674C(u8 *arg0);

void func_800641D0(volatile u32 *arg0, s32 arg1);

void func_800640D4(s32 arg0) {
    register s32 option asm("$5") = arg0;
    register s32 zero asm("$0");
    volatile u32 *inBuffer = (volatile u32 *)D_8008305C;

    if (option == 0) {
        goto zero;
    }
    if (option == 1) {
        goto one;
    }
    goto bad;

zero:
    *D_80083198 = 0x80000000;
    *D_8008316C = zero;
    *D_80083178 = zero;
    *D_80083198 = 0x60000000;
    func_800641D0(inBuffer, 0x20);
    func_800641D0((volatile u32 *)D_800830E0, 0x20);
    return;

one:
    *D_80083198 = 0x80000000;
    *D_8008316C = 0;
    *D_80083178 = 0;
    *D_80083178;
    *D_80083198 = 0x60000000;
    return;

bad:
    func_8001674C(D_800132C8);
}

void func_800641D0(volatile u32 *arg0, s32 arg1) {
    func_800642F4();
    *D_8008319C |= 0x88;
    *D_80083164 = (u32)(arg0 + 1);
    *D_80083168 = ((u32)arg1 >> 5 << 16) | 0x20;
    *D_80083194 = *arg0;
    *D_8008316C = 0x01000201;
}

void func_80064264(volatile u32 *arg0, s32 arg1) {
    func_8006438C();
    *D_8008319C |= 0x88;
    *D_80083178 = 0;
    *D_80083170 = (u32)arg0;
    *D_80083174 = ((u32)arg1 >> 5 << 16) | 0x20;
    *D_80083178 = 0x01000200;
}
