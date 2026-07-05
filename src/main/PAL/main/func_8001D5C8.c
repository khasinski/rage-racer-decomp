#include "common.h"

extern u16 D_8019CB38;

void func_8001D5C8(u32 arg0, u32 arg1) {
    u32 a;
    u32 b;

    a = arg0 >> 1;
    b = arg1 >> 1;
    a &= 0x3DEF;
    b &= 0x3DEF;
    D_8019CB38 = (a + b) | 0x8000;
}
