#include <sys/types.h>

#include "common.h"

u_long *DecDCTGetEnv(u_long *arg0);
u_long *DecDCTPutEnv(u_long *arg0);

u_long _new_card[4] asm("func_80063E34") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090050,
    0,
};

void func_8006DF34(void);
void func_800640D4(long arg0);

void DecDCTReset(long arg0) asm("func_80063E44");
void DecDCTReset(long arg0) {
    if (arg0 == 0) {
        func_8006DF34();
    }
    func_800640D4(arg0);
}

extern u_long D_80083060[];
extern u_long D_800830A0[];
extern u_long D_800830E4[];

u_long * DecDCTGetEnv(u_long *arg0) asm("func_80063E7C");
u_long *DecDCTGetEnv(u_long *arg0) {
    u_long *dst;
    u_long *src;
    long i;

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

extern u_char D_8008305C[];
extern u_char D_800830E0[];

void func_800641D0(volatile u_long *arg0, long arg1);

u_long * DecDCTPutEnv(u_long *arg0) asm("func_80063F08");
u_long *DecDCTPutEnv(u_long *arg0) {
    u_long *ret;
    u_long *dst;
    long i;

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

    func_800641D0((volatile u_long *)D_8008305C, 0x20);
    func_800641D0((volatile u_long *)D_800830E0, 0x20);

    return ret;
}

long DecDCTBufSize(u_short *arg0) asm("func_80063FA4");
long DecDCTBufSize(u_short *arg0) {
    return *arg0;
}

void DecDCTin(volatile u_long *arg0, long arg1) asm("func_80063FB0");
void DecDCTin(volatile u_long *arg0, long arg1) {
    u_long mask;

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

    func_800641D0(arg0, *(u_short *)arg0);
}
