#include "common.h"

extern s16 D_8019CB38;
extern s16 D_8019CB3A;
extern s16 D_8019CB3C;

void func_8001D6F4(u32 arg0, u32 arg1) {
    u32 a;
    u32 b;
    register u32 c asm("$2");
    register u32 high asm("$3");
    register s32 bias asm("$6");

    a = arg0 >> 1;
    b = arg1 >> 1;
    a &= 0x3DEF;
    b &= 0x3DEF;
    c = a + b;
    bias = 0x8000;
    __asm__ volatile("addu %0,%1,%2" : "=r"(high) : "r"(c), "r"(bias));
    c >>= 1;
    c &= 0x3DEF;
    a += c;
    a -= bias;
    c += b;
    c -= bias;

    D_8019CB3A = high;
    D_8019CB38 = a;
    D_8019CB3C = c;
}
