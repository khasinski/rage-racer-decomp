#include "common.h"

extern s16 D_8009E782;
extern s32 D_801E4D8C;
extern u8 *D_8019C7A4;

void func_8001F274(s32 arg0, u8 *arg1) {
    register u8 *dst asm("$2");
    register u8 *base asm("$3");
    u32 first;

    D_801E4D8C = D_8009E782;
    if (arg0 & 1) {
        return;
    }

    arg0 >>= 1;
    dst = (u8 *)(((arg0 << 3) - arg0) << 2);
    base = D_8019C7A4;
    first = *(u16 *)(arg1 + 0x0);
    dst += (s32)base;
    *(u16 *)(dst + 0x0) = first;
    *(u16 *)(dst + 0x2) = *(u16 *)(arg1 + 0x4);
    *(u16 *)(dst + 0x4) = *(u16 *)(arg1 + 0x8);
    *(u16 *)(dst + 0x6) = *(u16 *)(arg1 + 0x60);
    *(u16 *)(dst + 0x8) = *(u16 *)(arg1 + 0x20);
    *(u16 *)(dst + 0xA) = *(u16 *)(arg1 + 0x24);
    *(u16 *)(dst + 0xC) = *(u16 *)(arg1 + 0x28);
    *(u16 *)(dst + 0xE) = *(u16 *)(arg1 + 0x48);
    *(u16 *)(dst + 0x18) = *(u16 *)(arg1 + 0x44);
    *(u32 *)(dst + 0x14) = *(u32 *)(arg1 + 0x30);
    *(s32 *)(dst + 0x10) = *(s16 *)(arg1 + 0x8C);
}
