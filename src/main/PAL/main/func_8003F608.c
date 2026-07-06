#include "common.h"

extern s32 D_8009E6A4;

void func_8003E590(void);
void func_8003EAF4(void);
void func_8003EC98(void);
void func_8003F02C(void);
void func_8003F9C4(void);
void func_80040730(void);

void func_8003F608(s32 arg0) {
    switch (D_8009E6A4 % 5) {
    case 0:
        if (arg0 != 0) {
            func_8003EC98();
        }
        func_8003F02C();
        break;
    case 1:
    case 2:
        if (arg0 != 0) {
            func_8003EC98();
            func_8003E590();
        }
        func_8003F02C();
        func_8003EAF4();
        break;
    case 3:
    case 4:
        if (arg0 != 0) {
            func_8003EC98();
            func_8003E590();
            func_8003F9C4();
        }
        func_8003F02C();
        func_8003EAF4();
        func_80040730();
        break;
    case 5:
        break;
    }
}
