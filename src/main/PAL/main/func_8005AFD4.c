#include "common.h"

extern u8 D_801E8AFC;

s32 func_8007B014(void *arg0, s32 arg1);

s32 func_8005AFD4(void) {
    u8 *base;
    s32 value0;
    s32 value1;

    base = &D_801E8AFC;
    value1 = func_8007B014(base, 0);
    value1 = (value1 << 9) + (s32)base;
    value0 = *(s16 *)(value1 + 0x800);
    value1 = *(s16 *)(value1 + 0xC00);

    value0 = value0 < 0 ? -value0 : value0;
    value1 = value1 < 0 ? -value1 : value1;

    return (value0 << 16) | (s16)value1;
}
