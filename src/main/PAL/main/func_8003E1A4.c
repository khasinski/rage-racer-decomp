#include "common.h"

extern s32 D_8009E6A4;

void func_8003D6F0(s32 arg0, s32 arg1);
void func_8003DDAC(s32 arg0, s32 arg1);
void func_8003E0D0(void);
void func_8003DF68(s32 arg0);
void func_8003F2A4(s32 arg0);
void func_8003F4BC(s32 arg0);

void func_8003E1A4(s32 arg0, s32 arg1, s32 arg2) {
    register s32 mode asm("$16") = arg0;
    register s32 value asm("$18") = arg1;
    register s32 flag asm("$17");

    asm("" : "=r"(mode) : "0"(mode));
    asm("" : "=r"(value) : "0"(value));

    flag = arg2;

    func_8003D6F0(value, 0);

    if (D_8009E6A4 == 5) {
        flag = 0;
    }

    switch (mode) {
    case 0:
        func_8003DDAC(value, flag);
        if (D_8009E6A4 >= 4) {
            func_8003E0D0();
        }
        goto call0;
    case 1:
        if (D_8009E6A4 >= 2) {
            func_8003DDAC(value, flag);
        }
        if (flag != 0) {
            func_8003F2A4(0);
        }
        func_8003F4BC(0);
        goto call0;
    case 2:
        if (flag != 0) {
            func_8003F2A4(0);
            func_8003F2A4(1);
        }
        func_8003F4BC(0);
        func_8003F4BC(1);
        goto call0;
    case 3:
        func_8003D6F0(value, 1);
        goto call1;
    default:
        return;
    }

call0:
    func_8003DF68(0);
    return;
call1:
    func_8003DF68(1);
}
