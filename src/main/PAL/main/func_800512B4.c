#include "common.h"

extern u8 *D_8019C900;

s32 func_8001720C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_800512B4(s32 arg0) {
    s32 *scratch = (s32 *)0x1F800000;
    u8 *base = D_8019C900;
    s32 value = *scratch;

    *scratch = func_8001720C(base + 0xCC, value, 0x24, (arg0 << 4) + 0x24, 0x50, 0x10, 0, 0, 0xFF);
}
