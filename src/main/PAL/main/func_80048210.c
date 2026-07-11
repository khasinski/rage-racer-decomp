#include "common.h"

void func_8004711C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, u8 r, u8 g, u8 b, u8 alpha);

void func_80048210(s32 arg0, u8 *arg1, u8 *arg2) {
    register u8 *record asm("$8") = arg2;
    register u8 *style asm("$10");
    register void *otBase asm("$13");
    register s32 mode asm("$11");
    register s32 y1Reg asm("$2");
    register s32 x0Base asm("$6");
    register s32 x0 asm("$5");
    register s32 y0 asm("$6");
    register s32 y0Call asm("$12");
    register s32 y0Arg asm("$6");
    register s32 x1Base asm("$3");
    register s32 x1 asm("$7");
    register s32 y1 asm("$6");
    s32 limit;
    s32 xPacked;
    s32 yPacked;
    s32 temp;
    s32 interp;
    s32 alpha;

    /* Match note: materialize record in $t0 before the first load. */
    asm("" : "=r"(record) : "0"(record));
    limit = *(s32 *)record;
    otBase = *(void **)0x1F800004;
    xPacked = *(s32 *)(record + 0xC);
    yPacked = *(s32 *)(record + 0x10);
    style = arg1;
    if (limit < arg0) {
        arg0 = limit;
    }

    x0Base = *(s16 *)(record + 4);
    if (xPacked & 0x8000) {
        temp = xPacked | 0xFFFF0000;
    } else {
        temp = xPacked & 0x7FFF;
    }
    asm("mult %1,%0\n\tmflo %0\n\tsrl %0,%0,5" : "=r"(interp) : "r"(arg0), "0"(temp));
    x0 = x0Base + interp;

    y0 = *(s16 *)(record + 6);
    if (xPacked < 0) {
        register s32 hi asm("$2");
        register s32 mask asm("$3");

        hi = xPacked >> 16;
        mask = 0xFFFF0000;
        temp = hi | mask;
    } else {
        temp = (xPacked >> 16) & 0x7FFF;
    }
    asm("mult %1,%0\n\tmflo %0\n\tsrl %0,%0,5" : "=r"(interp) : "r"(arg0), "0"(temp));
    y0 += interp;
    asm("" : "=r"(y0) : "0"(y0));

    x1Base = *(s16 *)(record + 8);
    if (yPacked & 0x8000) {
        y0Call = y0;
        temp = yPacked | 0xFFFF0000;
    } else {
        y0Call = y0;
        temp = yPacked & 0x7FFF;
    }
    asm("mult %1,%0\n\tmflo %0\n\tsrl %0,%0,5" : "=r"(interp) : "r"(arg0), "0"(temp));

    y1 = *(s16 *)(record + 0xA);
    x1 = x1Base + interp;
    if (yPacked < 0) {
        register s32 hi asm("$2");
        register s32 mask asm("$3");

        hi = yPacked >> 16;
        mask = 0xFFFF0000;
        temp = hi | mask;
    } else {
        temp = (yPacked >> 16) & 0x7FFF;
    }
    interp = (u32)(arg0 * temp) >> 5;
    y1 += interp;
    asm("" : "=r"(y1) : "0"(y1));

    switch (style[3] & 3) {
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

    if (style[3] & 4) {
        alpha = style[3] & 0x60;
    } else {
        alpha = 0xFF;
    }

    y1Reg = (s16)y1;
    arg0 = mode << 2;
    x0 <<= 0x10;
    y0Arg = y0Call << 0x10;
    x1 <<= 0x10;
    /* Match note: keep the target operand order for the OT pointer add. */
    asm("addu %0,%1,%0" : "=r"(arg0) : "r"(otBase), "0"(arg0));
    func_8004711C(
        (void *)arg0,
        x0 >> 0x10,
        y0Arg >> 0x10,
        x1 >> 0x10,
        y1Reg,
        style[0],
        style[1],
        style[2],
        alpha);
}
