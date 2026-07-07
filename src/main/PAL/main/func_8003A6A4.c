#include "common.h"

extern u8 *D_801E40B8[];

void func_8003A6A4(u8 *arg0, s32 arg1) {
    u8 *entry;
    s32 offset;
    s32 pos0Base;
    s32 pos0;
    s32 pos1;
    s32 value;

    offset = arg1 << 2;
    pos0Base = *(s32 *)(arg0 + 0x68);
    entry = *(u8 **)((u8 *)D_801E40B8 + offset);
    pos0 = pos0Base + *(s32 *)(arg0 + 0x6C);
    pos1 = *(s32 *)(entry + 0x68) + *(s32 *)(entry + 0x6C);

    if ((pos1 - pos0) < 0x2800) {
        return;
    }

    if (*(s32 *)(entry + 0xA4) >= 0x385) {
        value = *(s16 *)(entry + 0x130);
        value = ((value * 5) + ((value * 5) << 4)) / 100;
        *(s16 *)(entry + 0x130) = value;
    }
}
