#include "common.h"

void func_80065198(u8 *arg0, u8 *arg1) {
    s32 sign;
    s32 value;
    s32 size;

    sign = *(s16 *)&arg1[4];
    value = sign * *(s16 *)&arg1[6];
    value += 1;
    sign = (u32)value >> 31;
    value += sign;
    value >>= 1;
    size = value + 4;
    if ((u32)(value - 1) >= 0xB) {
        size = 0;
    }

    *(u32 *)&arg0[4] = 0x01000000;
    arg0[3] = size;
    *(u32 *)&arg0[8] = 0xA0000000;
    *(u32 *)&arg0[0xC] = *(u32 *)&arg1[0];
    *(u32 *)&arg0[0x10] = *(u32 *)&arg1[4];
}

s32 func_800651FC(u8 *arg0, u8 *arg1) {
    s32 value;

    value = arg0[3] + arg1[3] + 1;
    if (value >= 0x21) {
        return -1;
    }
    arg0[3] = value;
    *(u32 *)arg1 = 0;
    return 0;
}
