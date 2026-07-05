#include "common.h"

u8 *SetDefDrawEnv(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_80064B78");

u8 *SetDefDrawEnv(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 *ret;

    ret = arg0;
    *(s16 *)&ret[0] = arg1;
    *(s16 *)&ret[2] = arg2;
    *(s16 *)&ret[4] = arg3;
    *(s16 *)&ret[8] = 0;
    *(s16 *)&ret[0xA] = 0;
    *(s16 *)&ret[0xC] = 0;
    *(s16 *)&ret[0xE] = 0;
    ret[0x11] = 0;
    ret[0x10] = 0;
    ret[0x13] = 0;
    ret[0x12] = 0;
    *(s16 *)&ret[6] = arg4;
    return ret;
}
