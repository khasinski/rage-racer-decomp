#include "common.h"

extern s16 D_801E6D90;
extern s32 D_801E6D84;
extern s32 D_801E6D88;
extern s32 D_801E6D8C;
extern s32 D_801E6D94;
extern s32 D_801E6D9C;

void func_8005B190(s32 arg0, s32 arg1);
void func_8005E600(s32 arg0);
void func_8005E7A0(s32 arg0);
void func_8005E8B8(void);
void func_80072698(s32 arg0, s32 arg1, s32 arg2);

void func_8005E900(void) {
    register s32 *fadeStep asm("$4");
    register s32 delta asm("$3");
    register s32 value asm("$2");
    register s32 arg0 asm("$4");
    register s32 arg1 asm("$5");

    fadeStep = &D_801E6D8C;
    asm volatile("" : "=r"(fadeStep) : "0"(fadeStep));
    delta = *fadeStep;
    if (delta != 0) {
        value = fadeStep[-2];
        value += delta;
        if ((fadeStep[-2] = value) < 0) {
            fadeStep[-2] = 0;
        }

        value = D_801E6D88;
        value += delta;
        D_801E6D88 = value;
        if (value < 0) {
            D_801E6D88 = 0;
        }

        if ((fadeStep[-2] == 0) && (D_801E6D88 == 0)) {
            *fadeStep = 0;
        }
    }

    func_8005B190(D_801E6D84, D_801E6D88);

    value = D_801E6D94;
    delta = D_801E6D9C;
    value += delta;
    D_801E6D94 = value;
    if (value <= 0) {
        D_801E6D94 = 0;
        D_801E6D9C = 0;
        func_8005E8B8();
        func_8005E600(6);
        arg0 = 0x28;
        arg1 = 0x28;
        func_8005B190(arg0, arg1);
    }

    func_8005E7A0(D_801E6D94);
}
asm(".globl func_8005E9F4\nfunc_8005E9F4 = func_8005E900 + 0xF4");

void func_8005EA14(void) {
    register s32 value asm("$3");
    register s32 scaled asm("$2");
    register s32 seq asm("$4");
    register s32 volume asm("$5");

    value = D_801E6D94;
    seq = D_801E6D90;
    asm volatile("" : : "r"(seq));
    scaled = value << 1;
    value = scaled + value;
    scaled = value;
    if (value < 0) {
        scaled = value + 3;
    }
    scaled <<= 0xE;
    volume = scaled >> 0x10;
    func_80072698(seq, volume, volume);
    func_8005B190(0x3C, 0x3C);
}
