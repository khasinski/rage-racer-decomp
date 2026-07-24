#include "common.h"

extern u8 *D_801E4B98;

u32 func_800414C4(s32 arg0, s32 arg1, s32 arg2) {
    s32 x = arg0 << 2;
    s32 y;
    u8 *base;
    u32 mask;

    y = arg1 << 7;
    base = D_801E4B98;
    mask = 1;
    y += (s32)base;
    x += y;
    return (mask << arg2) & *(u32 *)x;
}
