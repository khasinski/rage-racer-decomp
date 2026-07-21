#include "common.h"

#define SCRATCH (*(u8 **)0x1F800000)

extern u8 *D_801E42D8;
extern u8 *volatile D_8019C900;
extern s32 D_8009E778;
extern s16 D_8009E806;
extern u16 D_801E4130;
extern s16 D_8019C7D6;
extern u8 D_8007DF00;
extern u8 D_8007DF01;
extern u8 D_8007DF02;

s32 func_80068568(s32 angle);
s32 func_80068634(s32 angle);
void func_80064F30(void *prim);
void func_80064DDC(void *ot, void *prim);
void func_80064FF8(void *prim);
u8 *func_80033B7C(u8 *prim, s32 x, s32 y, s32 code, u16 arg4);
void func_80033C18(s32 x, s32 y, s32 value);

void func_8003351C(s32 a0, s32 arg1, s32 type, s32 amt) {
    u8 *p = D_801E42D8;
    s32 cx = *(s16 *)(p + 0x138);
    s32 cy = *(s16 *)(p + 0x13A);
    u8 *base = p + 0x138;
    s32 b = *(s16 *)(p + 0x150);
    s32 angle = b + a0 * (*(s16 *)(p + 0x152) - b) / 10000;
    s32 cos = func_80068568(angle);
    s32 sin = func_80068634(angle);
    u8 *prim = SCRATCH;
    s16 *vp;
    s16 *pa;
    s16 *pb;
    s32 i;
    u8 code7;

    func_80064F30(prim);

    vp = (s16 *)(prim + 8);
    i = 0;
    pb = &D_8019C7D6;
    pa = pb - 1;
    for (; i < 4; i++) {
        *vp++ = cx + (sin * pa[0] - cos * pb[0]) / 4096;
        *vp++ = cy + (cos * pa[0] + sin * pb[0]) / 4096;
        pb += 2;
        pa += 2;
    }

    code7 = prim[7];

    if (type == 1) {
        if (amt > 96) amt = 96;
        D_8007DF02 = -128 - amt;
        D_8007DF01 = -128 - amt;
        D_8007DF00 = -128 - amt;
        prim[4] = (amt * 32 + base[28] * (96 - amt)) / 96;
        prim[5] = (amt * 32 + base[29] * (96 - amt)) / 96;
        prim[6] = (amt * 32 + base[30] * (96 - amt)) / 96;
    } else if (type == 3) {
        amt -= 32;
        if (amt < 0) amt = 0;
        D_8007DF02 = amt + 32;
        D_8007DF01 = amt + 32;
        D_8007DF00 = amt + 32;
        prim[4] = ((96 - amt) * 32 + base[28] * amt) / 96;
        prim[5] = ((96 - amt) * 32 + base[29] * amt) / 96;
        prim[6] = ((96 - amt) * 32 + base[30] * amt) / 96;
        *(s16 *)(D_8019C900 + 0x236F2) = 0x33A8;
    } else if (type == 2) {
        *(s16 *)(D_8019C900 + 0x236F2) = 0x33E8;
        D_8007DF02 = 0x80;
        D_8007DF01 = 0x80;
        D_8007DF00 = 0x80;
        *(s32 *)(prim + 4) = *(s32 *)(base + 32);
    } else {
        s16 rv = 0x33A8;
        asm("" : "=r"(rv) : "0"(rv));
        *(s16 *)(D_8019C900 + 0x236F2) = rv;
        D_8007DF02 = 0x80;
        D_8007DF01 = 0x80;
        D_8007DF00 = 0x80;
        *(s32 *)(prim + 4) = *(s32 *)(base + 28);
    }

    prim[7] = code7;
    func_80064DDC(D_8019C900 + 0xCC, prim);
    prim += 24;
    SCRATCH = prim;

    {
        s32 x = cx + *(s16 *)(base + 12);
        s32 y = cy + *(s16 *)(base + 14);
        u8 *q = func_80033B7C(*(u8 *volatile *)0x1F800000, x, y, D_8009E806, D_801E4130);
        SCRATCH = q;
        func_80033C18(cx, cy, D_8009E778 * 160 / 1168);
    }

    *(u8 *)(D_8019C900 + 0x236E8) = D_8007DF00;
    *(u8 *)(D_8019C900 + 0x236E9) = D_8007DF01;
    *(u8 *)(D_8019C900 + 0x236EA) = D_8007DF02;

    { u8 *g = D_8019C900; func_80064DDC(g + 0xCC, g + 0x236CC); }
    { u8 *g = D_8019C900; func_80064DDC(g + 0xCC, g + 0x236E4); }
    { u8 *g = D_8019C900; func_80064DDC(g + 0xCC, g + 0x236D8); }

    {
        register u8 *q asm("$16") = SCRATCH;
        register u8 *g asm("$4");
        register u8 *prim asm("$5");
        s32 v10;
        func_80064FF8(q);
        prim = q;
        *(s16 *)(q + 8) = cx + *(u16 *)(base + 16);
        v10 = cy + *(u16 *)(base + 18);
        *(s16 *)(q + 12) = 0x10;
        *(s16 *)(q + 14) = 0x10;
        q[4] = arg1 * 223 + 32;
        q[5] = 0x20;
        q[6] = 0x20;
        g = D_8019C900;
        asm("" : "=r"(v10) : "0"(v10), "r"(g));
        *(s16 *)(q + 10) = v10;
        q += 16;
        asm("" : "=r"(g) : "0"(g), "r"(q) : "memory");
        func_80064DDC(g + 0xCC, prim);
        SCRATCH = q;
    }
}
