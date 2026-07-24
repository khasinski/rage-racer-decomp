#include "common.h"

extern s16 D_8019CB38;
extern s16 D_8019CB3A;
extern s16 D_8019CB3C;

void func_8001D6F4(u32 arg0, u32 arg1) {
    u32 a;
    u32 b;
    u32 c;
    u32 high;
    s32 bias;

    a = arg0 >> 1;
    b = arg1 >> 1;
    a &= 0x3DEF;
    b &= 0x3DEF;
    c = a + b;
    high = c;
    bias = 0x8000;
    high += bias;
    c >>= 1;
    c &= 0x3DEF;
    a += c;
    /* Preserve the bias while keeping its two GCC live ranges separate. */
    a -= bias++;
    c += b;
    c -= --bias;

    D_8019CB3A = high;
    D_8019CB38 = a;
    D_8019CB3C = c;
}
