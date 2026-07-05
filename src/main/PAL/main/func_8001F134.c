#include "common.h"

extern s16 D_8009E782;
extern s32 D_801E4D8C;
extern s32 D_801E4BC0;
extern u8 *D_8009F0A4;

void func_8001F134(s32 arg0, u8 *arg1, u8 *arg2) {
    register u8 *dst asm("$2");
    register u8 *base asm("$3");
    register s32 saved_ae asm("$3");
    register s32 current asm("$2");
    s32 odd;
    u32 first;

    current = D_8009E782;
    saved_ae = *(s16 *)(arg2 + 0xAE);
    D_801E4D8C = current;
    odd = arg0 & 1;
    D_801E4BC0 = saved_ae;
    if (odd) {
        return;
    }

    arg0 >>= 1;
    dst = (u8 *)(((arg0 << 1) + arg0) << 4);
    base = D_8009F0A4;
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
    *(u16 *)(dst + 0x2C) = *(u16 *)(arg1 + 0x44);
    *(u16 *)(dst + 0x10) = *(u16 *)(arg2 + 0x0);
    *(u16 *)(dst + 0x12) = *(u16 *)(arg2 + 0x4);
    *(u16 *)(dst + 0x14) = *(u16 *)(arg2 + 0x8);
    *(u16 *)(dst + 0x16) = *(u16 *)(arg2 + 0x60);
    *(u16 *)(dst + 0x18) = *(u16 *)(arg2 + 0x20);
    *(u16 *)(dst + 0x1A) = *(u16 *)(arg2 + 0x24);
    *(u16 *)(dst + 0x1C) = *(u16 *)(arg2 + 0x28);
    *(u16 *)(dst + 0x1E) = *(u16 *)(arg2 + 0x48);
    *(u16 *)(dst + 0x2E) = *(u16 *)(arg2 + 0x44);
    *(u32 *)(dst + 0x24) = *(u32 *)(arg1 + 0x30);
    *(u32 *)(dst + 0x28) = *(u32 *)(arg2 + 0x30);
    *(s32 *)(dst + 0x20) = *(s16 *)(arg1 + 0x8C);
}
