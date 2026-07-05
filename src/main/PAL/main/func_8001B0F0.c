#include "common.h"

extern u8 *D_8019C900;

void *func_8001720C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void func_8001B0F0(s32 arg0) {
    register void **scratch asm("$17");
    register u8 *base asm("$16");
    void *next;
    s32 color;

    scratch = (void **)0x1F800000;
    color = arg0 & 0xFF;
    base = D_8019C900;
    base += 0xCC;
    next = func_8001720C(base, *scratch, 0, 0x18, 0x140, 0xC0, color, color, color);
    *scratch = func_80017390(base, next, 0x29);
}
