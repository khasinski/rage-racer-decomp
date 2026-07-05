#include "common.h"

extern s32 D_8009E6A4;
extern s32 D_801E428C;
extern s32 D_801E42E4;

void func_8003DA90(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8003DDAC(s32 arg0, s32 arg1);
void func_8003E0D0(void);
void func_8003DF68(s32 arg0);
void func_8003F2A4(s32 arg0);
void func_8003F4BC(s32 arg0);

void func_8003E2E8(s32 arg0, s32 arg1) {
    register s32 value asm("$17") = arg0;
    register s32 flag asm("$16") = arg1;
    s32 mode;

    asm("" : "=r"(value) : "0"(value));
    asm("" : "=r"(flag) : "0"(flag));

    if (D_8009E6A4 == 5) {
        flag = 0;
    }

    func_8003DA90(value, 0, D_801E42E4 == 0x11, flag);

    mode = D_801E428C & 3;
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
        func_8003DA90(value, 1, D_801E42E4 == 0x11, flag);
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
