#include "common.h"

void func_80046BA0(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 arg10, s32 arg11);

void func_800483D4(s32 arg0, u8 *arg1, u8 *arg2) {
    register u8 *style asm("a1");
    register u8 *rec asm("a2");
    register void *scratch asm("t5");
    register s32 limit asm("a3");
    register s32 value asm("v1");
    register s32 product asm("v0");
    register s32 x0 asm("t1");
    register s32 x1 asm("t3");
    register s32 y0 asm("a2");
    register s32 y1 asm("t4");
    register s32 mode asm("t2");
    register s32 flag4 asm("t0");
    s32 scale;
    s32 stack10;
    s32 stack14;
    s32 stack18;
    s32 stack1c;
    s32 stack20;
    s32 stack24;
    s32 stack28;
    s32 stack2c;

    style = arg1;
    rec = arg2;
    limit = *(s32 *)rec;
    scratch = *(void **)0x1F800004;
    value = *(s32 *)(rec + 8);
    if (limit < arg0) {
        arg0 = limit;
    }

    limit = *(s16 *)(rec + 4);
    if (value & 0x8000) {
        scale = value | 0xFFFF0000;
    } else {
        scale = value & 0x7FFF;
    }
    product = arg0 * scale;
    x0 = limit + ((u32)product >> 5);

    y0 = *(s16 *)(rec + 6);
    if (value < 0) {
        scale = (value >> 16) | 0xFFFF0000;
    } else {
        scale = (value >> 16) & 0x7FFF;
    }
    product = arg0 * scale;
    y0 += (u32)product >> 5;

    x1 = *(u16 *)(style + 2) + y0;
    y1 = *(u16 *)(style + 6) + y0;
    limit = *(u16 *)(style + 0) + x0;
    value = *(u16 *)(style + 4) + x0;

    switch (style[0xB] & 3) {
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 3;
        break;
    case 2:
        mode = 5;
        break;
    case 3:
        mode = 0x2BE;
        break;
    }

    flag4 = style[0xB] & 4;
    if (flag4 != 0) {
        stack2c = style[0xB] & 0x60;
        stack2c = (u8)stack2c;
    } else {
        stack2c = 0x80;
    }

    stack10 = (s16)x1;
    stack14 = (s16)value;
    stack18 = (s16)y1;
    stack1c = style[8];
    stack20 = style[9];
    stack24 = style[0xA];
    stack28 = flag4;

    func_80046BA0((void *)((s32)scratch + (mode << 2)), (s16)x0, (s16)y0, (s16)limit, stack10, stack14, stack18,
                  stack1c, stack20, stack24, stack28, stack2c);
}
