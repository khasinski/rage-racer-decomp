#include "common.h"

extern u8 *D_801E413C;
extern u8 D_801E4BD4;
extern u8 D_801E4BDA;
extern u8 D_801E4BDD;
extern u8 D_801E4BDB;
extern u8 D_801E4BD5;
extern u8 D_801E4BDE;
extern u8 D_801E4BE4;
extern u16 D_801E4BE6;
extern s16 D_801E4BEA;
extern s16 D_801E3FB0;
extern u8 *D_801E79CC[];
extern u8 D_8009DF20[];
extern u8 D_8009E0A0[];
extern u8 D_8009E0B8[];
extern u16 D_8009E680;
extern u16 D_8009E684;
extern u16 D_8009E670;
extern u16 D_8009E674;
extern u16 D_801F2A08;
extern u16 D_801F2A0C;

void func_80073C50(s32 arg0, s32 val) {
    u32 a1v, a2v, a3v;
    u16 vidx;
    s32 g, t, sixteen, center, hi;
    u8 *base;
    u32 kx;
    s16 *fp;
    s32 tA, F0;

    tA = D_801E413C[0x18] * 16383;
    a2v = D_801E4BD4 * tA / 16129;
    a3v = a2v * D_801E4BDA * D_801E4BDD / 16129;

    F0 = D_801E4BEA;
    vidx = F0 << 3;

    g = D_801E4BE6;
    center = g & 0xff;
    {
        s32 idx4 = center << 2;
        t = g << 16;
        sixteen = t >> 16;
        hi = (u32)t >> 24;
        base = *(u8 **)((u8 *)D_801E79CC + idx4) + hi * 172;
    }

    a2v = a3v;
    if (sixteen != 0x21) {
        asm("" : "=r"(a3v) : "0"(a3v));
        a2v = a3v * *(u16 *)(base + 116) / 127;
        a3v = a3v * *(u16 *)(base + 118) / 127;
    }

    kx = D_801E4BDE;
    if (kx < 64) {
        a1v = a3v * kx / 63;
    } else {
        a2v = a2v * (127 - kx) / 63;
        a1v = a3v;
    }

    kx = D_801E4BDB;
    if (kx < 64) {
        a1v = a1v * kx / 63;
    } else {
        a2v = a2v * (127 - kx) / 63;
    }

    kx = D_801E4BD5;
    if (kx < 64) {
        a1v = a1v * kx / 63;
    } else {
        a2v = a2v * (127 - kx) / 63;
    }

    if (D_801E3FB0 == 1) {
        if (a2v < a1v) {
            a2v = a1v;
        } else {
            a1v = a2v;
        }
    }

    a2v = a2v * a2v / 16383;
    a1v = a1v * a1v / 16383;

    *(u16 *)&D_8009DF20[vidx * 2 + 4] = val;
    *(u16 *)&D_8009DF20[vidx * 2] = a2v;
    *(u16 *)&D_8009DF20[vidx * 2 + 2] = a1v;

    fp = &D_801E4BEA;
    D_8009E0A0[*fp] |= 7;
    *(u16 *)&D_8009E0B8[*fp * 0x34 + 4] = val;
    D_8009E0B8[*fp * 0x34 + 0x1B] = 1;

    {
        s32 f = *fp;
        if (f < 16) {
            a2v = 1 << f;
            a1v = 0;
        } else {
            a2v = 0;
            a1v = 1 << (f - 16);
        }
    }

    if (D_801E4BE4 & 4) {
        D_8009E680 = a2v | D_8009E680;
        D_8009E684 = a1v | D_8009E684;
    } else {
        D_8009E680 &= ~a2v;
        D_8009E684 &= ~a1v;
    }

    D_8009E670 = a2v | D_8009E670;
    D_8009E674 = a1v | D_8009E674;
    D_801F2A08 &= ~D_8009E670;
    D_801F2A0C &= ~D_8009E674;

    (void)arg0;
}
