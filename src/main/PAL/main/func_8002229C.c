#include "common.h"

extern s32 D_8009E694;
extern u8 *D_8019C900;

s32 func_80032F34(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_8002229C(s32 arg0, s32 arg1) {
    register s32 *scratch asm("$16");

    if (D_8009E694 & 8) {
        scratch = (s32 *)0x1F800000;
        *scratch = func_80032F34(
            D_8019C900 + 0xCC,
            *scratch,
            (arg0 << 3) + 0x7C,
            (((arg1 << 2) + arg1) << 2) + 0x7E,
            9,
            2,
            0xC0,
            0x48,
            0x48);
    }
}

asm(".globl func_800222F4\nfunc_800222F4 = func_8002229C + 0x58");
