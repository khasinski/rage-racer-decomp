#include "common.h"

void func_80046E00(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17,
                   s32 arg18, s32 arg19, s32 arg20, s32 arg21, s32 arg22, s32 arg23);

void func_80048580(s32 arg0, u8 *arg1, u8 *arg2) {
    register u8 *style asm("t0");
    register u8 *rec asm("a2");
    register void *scratch asm("t5");
    register s32 limit asm("a3");
    register s32 packed1 asm("t1");
    register s32 x0_base asm("a1");
    register s32 x0 asm("a3");
    register s32 y0_base asm("a1");
    register s32 y0 asm("t3");
    register s32 x1 asm("t4");
    register s32 y1_base asm("a2");
    register s32 y1 asm("t1");
    register s32 mode asm("t2");
    s32 packed0;
    s32 scale;
    u32 product;
    s32 flag_byte;
    s32 x2;
    s32 y2;

    style = arg1;
    rec = arg2;
    limit = *(s32 *)rec;
    scratch = *(void **)0x1F800004;
    packed0 = *(s32 *)(rec + 0xC);
    packed1 = *(s32 *)(rec + 0x10);
    if (limit < arg0) {
        arg0 = limit;
    }

    x0_base = *(s16 *)(rec + 4);
    if (packed0 & 0x8000) {
        scale = packed0 | 0xFFFF0000;
    } else {
        scale = packed0 & 0x7FFF;
    }
    product = arg0 * scale;
    x0 = x0_base + (product >> 5);

    y0_base = *(s16 *)(rec + 6);
    if (packed0 < 0) {
        scale = (packed0 >> 16) | 0xFFFF0000;
    } else {
        scale = (packed0 >> 16) & 0x7FFF;
    }
    product = arg0 * scale;
    y0 = y0_base + (product >> 5);

    x1 = *(s16 *)(rec + 8);
    if (packed1 & 0x8000) {
        scale = packed1 | 0xFFFF0000;
    } else {
        scale = packed1 & 0x7FFF;
    }
    product = arg0 * scale;
    x1 += product >> 5;

    y1_base = *(s16 *)(rec + 0xA);
    if (packed1 < 0) {
        scale = (packed1 >> 16) | 0xFFFF0000;
    } else {
        scale = (packed1 >> 16) & 0x7FFF;
    }
    product = arg0 * scale;
    y1_base += product >> 5;

    switch (style[0xD] & 3) {
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

    y1 = y1_base;
    x2 = x0 + x1;
    y2 = y0 + y1;
    flag_byte = style[0xD];

    func_80046E00((void *)((s32)scratch + (mode << 2)), (s16)x0, (s16)y0, (s16)x2, (s16)y0, (s16)x0, (s16)y2,
                  (s16)x2, (s16)y2, style[0], style[1], style[2], style[3], style[4], style[5], style[6], style[7],
                  style[0xA], style[0xB], style[0xC], *(u16 *)(style + 8), flag_byte & 8, flag_byte & 4,
                  style[0xE]);
}
