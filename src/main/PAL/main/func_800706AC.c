#include "common.h"

typedef struct {
    /*00*/ u8  f00;
    /*01*/ u8  f01;
    /*02*/ u8  r02[4];
    /*06*/ u8  f06;
    /*07*/ u8  f07;
    /*08*/ u8  r08;
    /*09*/ u8  f09;
    /*0a*/ u8  f0a;
    /*0b*/ u8  r0b[5];
    /*10*/ u16 f10;
    /*12*/ u16 f12;
    /*14*/ u8  r14[0x0c];
    /*20*/ u16 f20;
    /*22*/ u16 f22;
    /*24*/ u16 f24;
    /*26*/ u16 f26;
    /*28*/ u16 f28;
    /*2a*/ u16 f2a;
    /*2c*/ u16 f2c;
    /*2e*/ u16 f2e;
    /*30*/ u16 f30;
    /*32*/ u16 r32;
} Arg;

void func_800733D8(s16, s16, s16, Arg *);
void func_80073820(s16, s16, s16, Arg *);
void func_80070E28(u16, u16, u16 *);
void func_80070E84(u16 *, u16 *, u16 *);
void func_80073614(s32);
void func_80073748(s32, s32);
void func_80073708(s32);
void func_800737E0(s32);
void func_800736E8(void);
void func_800736C8(void);

void func_800706AC(s16 p0, s16 p1, s16 p2, Arg arg, s16 mode, u8 val) {
    func_800733D8(p0, p1, p2, &arg);

    switch (mode) {
    case 0:
        arg.f00 = val;
        func_80073820(p0, p1, p2, &arg);
        break;
    case 1:
        arg.f01 = val;
        func_80073820(p0, p1, p2, &arg);
        if (val == 0) {
            func_800736E8();
            break;
        }
        if (val == 1) {
            break;
        }
        if (val == 2) {
            break;
        }
        if (val == 3) {
            break;
        }
        if (val == 4) {
            func_800736C8();
            break;
        }
        break;
    case 2:
        arg.f06 = val;
        func_80073820(p0, p1, p2, &arg);
        break;
    case 3:
        arg.f07 = val;
        func_80073820(p0, p1, p2, &arg);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        func_80070E28(arg.f10, arg.f12, &arg.f20);
        switch (mode) {
        case 4:
            arg.f2a = 0;
            arg.f20 = val;
            break;
        case 5:
            arg.f2a = 1;
            arg.f20 = val;
            break;
        case 6:
            arg.f22 = val;
            break;
        case 7:
            arg.f24 = val;
            break;
        case 8:
            arg.f2c = 0;
            arg.f26 = val;
            break;
        case 9:
            arg.f2c = 1;
            arg.f26 = val;
            break;
        case 10:
            arg.f2e = 0;
            arg.f28 = val;
            break;
        case 11:
            arg.f2e = 1;
            arg.f28 = val;
            break;
        case 12:
            if (val != 0 && val < 64) {
                arg.f30 = 0;
            } else if ((u8)(val - 64) < 64) {
                arg.f30 = 1;
            }
            break;
        case 13:
            arg.f09 = val;
            break;
        case 14:
            arg.f0a = val;
            break;
        }
        func_80070E84(&arg.f20, &arg.f10, &arg.f12);
        func_80073820(p0, p1, p2, &arg);
        break;
    case 15:
        func_80073614(val);
        break;
    case 16: {
        register s32 a0r asm("$4") = val;
        register s32 a1r asm("$5") = a0r;
        func_80073748(a0r, a1r);
        break;
    }
    case 17:
        func_80073708(val);
        break;
    case 18:
    case 19:
        func_800737E0(val);
        break;
    case 20:
    case 21:
    case 22:
        break;
    }
}
