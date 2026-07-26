#include "common.h"

u32 *DecDCTGetEnv(u32 *arg0);
u32 *DecDCTPutEnv(u32 *arg0);

u32 func_80063E34[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090050,
    0,
};

void func_8006DF34(void);
void func_800640D4(s32 arg0);

void DecDCTReset(s32 arg0) asm("func_80063E44");
void DecDCTReset(s32 arg0) {
    if (arg0 == 0) {
        func_8006DF34();
    }
    func_800640D4(arg0);
}

extern u32 D_80083060[];
extern u32 D_800830A0[];
extern u32 D_800830E4[];

u32 * DecDCTGetEnv(u32 *arg0) asm("func_80063E7C");
u32 *DecDCTGetEnv(u32 *arg0) {
    u32 *dst;
    u32 *src;
    s32 i;

    dst = arg0;
    src = D_80083060;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *src++;
    }

    dst = arg0 + 0x10;
    src = D_800830A0;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *src++;
    }

    dst = arg0 + 0x20;
    src = D_800830E4;
    for (i = 0x1F; i != -1; i--) {
        *dst++ = *src++;
    }

    return arg0;
}

extern u8 D_8008305C[];
extern u8 D_800830E0[];

void func_800641D0(volatile u32 *arg0, s32 arg1);

u32 * DecDCTPutEnv(u32 *arg0) asm("func_80063F08");
u32 *DecDCTPutEnv(u32 *arg0) {
    u32 *ret;
    u32 *dst;
    s32 i;

    ret = arg0;
    dst = D_80083060;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *arg0++;
    }

    dst = D_800830A0;
    arg0 = ret + 0x10;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *arg0++;
    }

    func_800641D0((volatile u32 *)D_8008305C, 0x20);
    func_800641D0((volatile u32 *)D_800830E0, 0x20);

    return ret;
}

s32 DecDCTBufSize(u16 *arg0) asm("func_80063FA4");
s32 DecDCTBufSize(u16 *arg0) {
    return *arg0;
}

void DecDCTin(volatile u32 *arg0, s32 arg1) asm("func_80063FB0");
void DecDCTin(volatile u32 *arg0, s32 arg1) {
    u32 mask;

    if (arg1 & 1) {
        arg0[0] &= 0xF7FFFFFF;
    } else {
        arg0[0] |= 0x08000000;
    }

    mask = 0x02000000;
    if (arg1 & 2) {
        arg0[0] |= mask;
    } else {
        arg0[0] &= 0xFDFFFFFF;
    }

    func_800641D0(arg0, *(u16 *)arg0);
}
