#include "common.h"

void func_80046A2C(
    void *ot,
    s32 x,
    s32 y,
    s32 arg3,
    s32 arg4,
    s32 arg5,
    s32 arg6,
    s32 arg7,
    s32 arg8,
    s32 arg9,
    s32 arg10,
    s32 arg11,
    s32 arg12,
    s32 arg13);

void func_80048078(s32 arg0, u8 *arg1, u8 *arg2, s32 arg3) {
    register u8 *record asm("$10") = arg2;
    register u8 *style asm("$9");
    register void *otBase asm("$12");
    register s32 mode asm("$11");
    register s32 flags4 asm("$8");
    register s32 flags8 asm("$4");
    register s32 limit asm("$8");
    s32 packed;
    s32 x;
    s32 y;
    s32 temp;
    s32 interp;
    register s32 flagByte asm("$2");
    s32 alpha;

    /* Match note: materialize record in $t2 before the first load. */
    asm("" : "=r"(record) : "0"(record));
    limit = *(s32 *)record;
    otBase = *(void **)0x1F800004;
    packed = *(s32 *)(record + 0x10);
    style = arg1;
    if (limit < arg0) {
        arg0 = limit;
    }

    x = *(s16 *)(record + 4);
    if (packed & 0x8000) {
        temp = packed | 0xFFFF0000;
    } else {
        temp = packed & 0x7FFF;
    }
    asm("mult %1,%0\n\tmflo %0\n\tsrl %0,%0,5" : "=r"(interp) : "r"(arg0), "0"(temp));

    y = *(s16 *)(record + 6);
    x += interp;
    if (packed < 0) {
        register s32 hi asm("$2");
        register s32 mask asm("$3");

        hi = packed >> 16;
        mask = 0xFFFF0000;
        temp = hi | mask;
    } else {
        temp = (packed >> 16) & 0x7FFF;
    }
    interp = (u32)(arg0 * temp) >> 5;
    y += interp;
    asm("" : "=r"(y) : "0"(y));

    switch (style[6] & 3) {
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

    flagByte = style[6];
    flags8 = flagByte & 8;
    flags4 = flagByte & 4;
    if (arg3 != 0) {
        temp = style[7] & 0x7F;
        asm("" : "=r"(temp) : "0"(temp));
        alpha = (u8)temp;
    } else {
        alpha = 0x80;
    }

    func_80046A2C(
        (u8 *)otBase + (mode * 4),
        (s16)x,
        (s16)y,
        *(s16 *)(style + 0),
        *(s16 *)(style + 2),
        style[4],
        style[5],
        record[0xA],
        record[0xB],
        record[0xC],
        *(u16 *)(record + 8),
        flags8,
        flags4,
        alpha);
}
