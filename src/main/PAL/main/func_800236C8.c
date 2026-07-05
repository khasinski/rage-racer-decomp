#include "common.h"

extern u8 *D_8019C900;

s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);

void func_800236C8(s32 arg0, s32 arg1) {
    register u8 *base asm("$16") = D_8019C900;
    register s32 *scratch asm("$17") = (s32 *)0x1F800000;
    s32 value;

    base += 0xCC;
    value = func_80017138((s32)base, *scratch, arg0, arg1, 0xC, 0x18, 0xE0, 0x48, 0x7F40);
    *scratch = func_80017390((s32)base, value, 0x3F);
}
