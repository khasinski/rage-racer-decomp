#include "common.h"

typedef struct {
    u16 f0;
    u16 f2;
    u16 pad4[4];
    u16 f12;
    u16 f14;
    u16 f16;
    u16 f18;
    u16 f20;
    u16 f22;
    u8  pad24[3];
    u8  f27;
    u8  pad28[24];
} Elem;

typedef struct {
    u8  f0, f1, f2, f3, f4, f5, f6, f7;
    u8  pad8[14];
    u16 f22;
    u8  pad24[8];
} Rec2;

extern s32 D_801E40AC;
extern u16 D_801E4BE6;
extern u8  D_801E4BD0;
extern u8  D_801E4BD2;
extern u8  D_801E4BD3;
extern u8  D_801E4BD4;
extern u8  D_801E4BD5;
extern u8  D_801E4BD7;
extern u8  D_801E4BDA;
extern u8  D_801E4BDB;
extern u8  D_801E4BDC;
extern u8  D_801E4BDD;
extern u8  D_801E4BDE;
extern u8  D_801E4BDF;
extern u8  D_801E4BE0;
extern u8  D_801E4BE1;
extern u8  D_801E4BE2;
extern u8  D_801E4BE3;
extern u8  D_801E4BE4;
extern u16 D_801E4BE8;
extern u16 D_801E4BEA;
extern u8  *D_801E4110;
extern Rec2 *D_801E416C;
extern Elem D_8009E0B8[];

extern int func_80073314(int, int);
extern void func_80074134(void);
extern void func_80074348(int);
extern int func_80074A6C(int, int);
extern void func_80073C50(int, int);

s32 func_80077C7C(s32 p1, s16 p2, s16 p3, s16 p4, u16 p5, u16 p6, s16 p7, s16 p8) {
    int idx;
    u8 bdc;
    u16 be8;
    u8 *q;
    Rec2 *r;
    int a, b;

    if (D_801E40AC == 1) {
        return -1;
    }
    D_801E40AC = 1;
    if ((u16)p1 >= 24) {
        D_801E40AC = 0;
        return -1;
    }
    if (func_80073314((s16)p2, (s16)p3) != 0) {
        D_801E40AC = 0;
        return -1;
    }
    D_801E4BE6 = 0x21;
    a = (s16)p7;
    b = (s16)p8;
    D_801E4BD2 = (u8)p5;
    D_801E4BD3 = (u8)p6;
    D_801E4BDC = (u8)p4;
    if (a == b) {
        D_801E4BD5 = 0x40;
        D_801E4BD4 = (u8)p7;
    } else if (b < a) {
        D_801E4BD4 = (u8)p7;
        D_801E4BD5 = (u8)((b << 6) / a);
    } else {
        D_801E4BD4 = (u8)p8;
        D_801E4BD5 = (u8)(0x7f - (a << 6) / b);
    }
    q = D_801E4110 + (s16)p3 * 16;
    D_801E4BDA = q[1];
    D_801E4BDB = q[4];
    D_801E4BD0 = q[0];
    r = &D_801E416C[D_801E4BDC + D_801E4BD7 * 16];
    D_801E4BDF = r->f0;
    asm("");
    be8 = r->f22;
    D_801E4BE8 = be8;
    D_801E4BDD = r->f2;
    D_801E4BDE = r->f3;
    D_801E4BE0 = r->f4;
    D_801E4BE1 = r->f5;
    D_801E4BE4 = r->f1;
    D_801E4BE2 = r->f6;
    D_801E4BE3 = r->f7;
    if (be8 == 0) {
        D_801E40AC = 0;
        return -1;
    }
    idx = (s16)p1;
    D_801E4BEA = (u16)p1;
    D_8009E0B8[idx].f14 = 0x21;
    asm("");
    D_8009E0B8[idx].f22 = (u16)p2;
    asm("");
    D_8009E0B8[idx].f18 = (u16)p3;
    D_8009E0B8[idx].f16 = D_801E4BD7;
    asm volatile("" ::: "memory");
    D_8009E0B8[idx].f0 = D_801E4BE8;
    asm("");
    bdc = D_801E4BDC;
    D_8009E0B8[idx].f12 = p5;
    D_8009E0B8[idx].f27 = 1;
    D_8009E0B8[idx].f2 = 0;
    D_8009E0B8[idx].f20 = bdc;
    func_80074134();
    if ((s16)D_801E4BE8 == 0xff) {
        func_80074348(p1 & 0xff);
    } else {
        func_80073C50(1, func_80074A6C(p5, p6) & 0xffff);
    }
    D_801E40AC = 0;
    return (s16)p1;
}
