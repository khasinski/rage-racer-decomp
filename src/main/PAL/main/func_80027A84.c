#include "common.h"

extern u8 *volatile D_8019C900;

s32 func_80017138(s32 base, s32 prim, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 color);
s32 func_80032F34(s32 base, s32 prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b);
void func_800236C8(s32 a, s32 b);
void func_80023750(s32 a);
void func_8002390C(void);
s32 func_800279EC(s32 base, s32 prim, s32 a, s32 b);

void func_80027A84(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 base = ((s32) D_8019C900) + 0xCC;
    s32 *scratch = (s32 *) 0x1F800000;
    s32 next;
    s32 i;
    s32 y;

    next = func_80017138(base, *scratch, 0x24, 0x38, 0x20, 0x18, 0xA0, 0x90, 0x7F40);
    if (arg1 != 0) {
        next = func_80017138(base, next, 0x24, 0x58, 0x24, 0x18, 0xCC, 0x90, 0x7F40);
    }
    if (arg1 != 0) {
        y = 0x78;
    } else {
        y = 0x58;
    }
    next = func_80017138(base, next, 0x24, y, 0x1C, 0x18, 0xD0, 0x60, 0x7F40);
    next = func_80017138(base, next, 0x48, 0xB8, 0x10, 0x10, 0, 0xC8, 0x7F40);
    next = func_80017138(base, next, 0x68, 0xB8, 0x34, 0x10, 0x10, 0xC8, 0x7F40);
    next = func_80017138(base, next, 0xB0, 0xB8, 0x14, 0x10, 0x44, 0xC8, 0x7F40);
    *scratch = next;
    func_800236C8(0x14, (arg2 << 5) + 0x38);
    func_80023750(arg1 + 5);
    func_8002390C();

    base = ((s32) D_8019C900) + 0xD8;
    next = func_80032F34(base, *scratch, 0x5D, 0x3C, 0xE4, 0x40, 0, 0, 0);
    next = func_80032F34(base, next, 0x5C, 0x3A, 0xE5, 0x44, 0xFF, 0xFF, 0xFF);
    for (i = 0; i < 3; i++) {
        next = func_800279EC(base, next, 0x3E, 0xD0 + i * 0x30);
    }

    if (arg0 != 0) {
        next = func_80032F34(base, next, 0x3C, ((arg3 * 3) << 4) + 0xCC, 0xC8, 0x28, 0x89, 0xFF, 0x76);
    }
    next = func_80032F34(base, next, 0, 0, 0x140, 0xF0, 0x85, 0x15, 0xE);
    *(s32 *)0x1F800000 = next;
}
