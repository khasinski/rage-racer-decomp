#include "common.h"

extern u8 *D_8019C900;

s32 func_80016EC4(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);

void func_800230B0(void) {
    register u8 *base asm("$16");
    register s32 clut asm("$17");
    register s32 height asm("$18");
    register volatile s32 *scratch asm("$19");
    s32 next;

    base = D_8019C900 + 0xCC;
    height = 0xF0;
    clut = 0x3FDB;
    scratch = (volatile s32 *)0x1F800000;

    next = func_80016EC4(base, *scratch, 0, 0, 0x100, height, 0, 0, clut);
    next = func_80017390(base, next, 6);
    next = func_80016EC4(base, next, 0x100, 0, 0x40, height, 0, 0, clut);
    *scratch = func_80017390(base, next, 7);
}
