#include "common.h"

extern s32 D_801E40B8;
extern s32 D_8019CA6C;
extern u8 D_801F18CC[];

s32 func_800632B0(void);
s32 func_80019D24(s32 arg0);

s32 func_8001A1F0(s32 mask, s32 current) {
    s32 random;
    s32 candidate;
    s32 offset;
    s32 first;

    if (mask & D_801E40B8) {
        return current;
    }
    if ((D_8019CA6C == 0) || (D_8019CA6C == 0x100)) {
        random = func_800632B0() & 0x7FFF;
        candidate = random % 4;

        offset = (((((current * 3) * 4) + current) * 8) - current) * 4;
        first = func_80019D24(*(s16 *)&D_801F18CC[offset]);

        offset = (((((candidate * 3) * 4) + candidate) * 8) - candidate) * 4;
        if (first == func_80019D24(*(s16 *)&D_801F18CC[offset])) {
            asm(".globl func_8001A2B8\nfunc_8001A2B8:");
            return candidate;
        }
    }
    return current;
}
