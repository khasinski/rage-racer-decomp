#include "common.h"

extern u8 *D_8019C900;
extern u8 D_8007D588[];
extern u8 D_8007D589[];
extern u8 D_8007D58A[];
extern u8 D_8007D58B[];

s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);

void func_80023750(s32 arg0) {
    register s32 index asm("$16");
    register s32 y asm("$17");
    register s32 which asm("$18");
    register s32 base asm("$19");
    register s32 color asm("$20");
    register s32 h18 asm("$21");
    register s32 y78 asm("$22");
    register s32 w0c asm("$23");
    register s32 raw_base asm("$2");
    register s32 scratch asm("$5");
    register s32 width asm("$3");
    s32 next;

    raw_base = (s32)D_8019C900;
    scratch = *(s32 *)0x1F800000;
    next = scratch;
    which = arg0;
    base = raw_base + 0xCC;

    if (which == 4) {
        y = 0x5A;
    } else {
        asm("" : "=r"(which) : "0"(which));
        width = D_8007D58A[which * 4];
        raw_base = 0x120;
        raw_base = raw_base - width;
        width = (u32)raw_base >> 0x1F;
        raw_base = raw_base + width;
        y = raw_base >> 1;
    }

    w0c = 0xC;
    h18 = 0x18;
    y78 = 0x78;
    color = 0x7F40;

    next = func_80017138(base, next, y, 0x180, w0c, h18, 0xE0, y78, color);

    y += 0x10;
    index = which * 4;
    next = func_80017138(base, next, y, 0x180, D_8007D58A[index], h18, D_8007D588[index], D_8007D589[index], color);

    y += D_8007D58B[index];
    if (which == 4) {
        next = func_80017138(base, next, y, 0x180, 0x30, h18, 0, y78, color);
        y += 0x34;
    }

    next = func_80017138(base, next, y, 0x180, w0c, h18, 0xEC, y78, color);
    *(s32 *)0x1F800000 = func_80017390(base, next, 0x3F);
}
