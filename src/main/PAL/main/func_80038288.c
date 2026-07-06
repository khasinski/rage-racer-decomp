#include "common.h"

extern s32 D_801E40D8;

s32 func_8001A6AC(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);

s32 func_80038288(s32 arg0) {
    register s32 trackLength asm("a1");
    register s32 value asm("v1");
    register s32 temp asm("v0");
    register s32 angle asm("s0");
    register s32 remainder asm("a0");
    register s32 scale asm("v1");

    trackLength = D_801E40D8;
    temp = 0xB875;
    value = trackLength + temp;
    value -= arg0;
    remainder = value % trackLength;
    value = remainder;

    if (value < 0x1A97) {
        temp = value / 100;
        value = 0x80 - temp;
    } else {
        value = trackLength - remainder;
        if (value < 0x801) {
            temp = value / 10;
            value = 0x80 - temp;
        } else {
            value = 0;
        }
    }

    if (value != 0) {
        temp = func_8001A6AC(0x29DD - *(s32 *)0x1F800008, 0x6EF3 - *(s32 *)0x1F800010);
        value = 0xC00 - temp;
        temp = *(s32 *)0x1F80001C;
        value &= 0xFFF;
        temp -= value;
        angle = temp & 0xFFF;
        temp = func_80068568(angle);
        temp = func_80068568(angle);
        scale = -0x40;
        __asm__ volatile("mult %0,%1" : : "r"(temp), "r"(scale));
    }
}

asm(".globl func_80038368\nfunc_80038368 = func_80038288 + 0xE0");
