#include "common.h"

extern u8 * volatile D_8019C900;
extern volatile u8 D_801E4369;
extern u8 D_8007D5A4;

s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);

void func_8002390C(void) {
    register s32 color asm("$16");
    register s32 y2 asm("$17");
    register s32 h asm("$18");
    register s32 w asm("$19");
    register s32 y0 asm("$20");
    register s32 base asm("$21");
    register s32 raw_base asm("$2");
    register s32 value asm("$3");
    register s32 scratch asm("$5");
    s32 next;

    raw_base = (s32)D_8019C900;
    value = D_801E4369;
    scratch = *(s32 *)0x1F800000;
    next = scratch;
    base = raw_base + 0xCC;

    if (value == 0x41) {
        goto use_current;
    }
    if (value == 0x23) {
        goto use_current;
    }

    value = D_8007D5A4;
    goto have_value;

use_current:
    raw_base = D_801E4369;
    asm("" : "=r"(raw_base) : "0"(raw_base));
    value = raw_base & 0xFF;
    D_8007D5A4 = raw_base;

have_value:

    y0 = value == 0x23 ? 0xA0 : 0x90;
    w = 8;
    h = 0x10;
    y2 = 0xB8;
    color = 0x7F40;

    next = func_80017138(base, next, 0x7A, 0x1A0, w, h, y0, y2, color);
    next = func_80017138(base, next, 0x92, 0x1A0, w, h, y0 + 8, y2, color);
    asm("" : : "r"(y0));
    next = func_80017138(base, next, 0x58, 0x1A0, 0x90, h, 0, y2, color);
    *(s32 *)0x1F800000 = func_80017390(base, next, 0x3F);
}
