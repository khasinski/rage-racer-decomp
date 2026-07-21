#include "common.h"

typedef struct SubB {
    char _p0[0x1C];
    s32 x1C;
    char _p1[0x20];
    s16 x40;
} SubB;

typedef struct A {
    char _a0[0x24];
    s32 f24;
    char _a1[0x0C];
    s32 f34;
    char _a2[0x0C];
    s32 f44;
    char _a3[0x1C];
    s32 f64;
    char _a4[0x3C];
    s32 fA4;
    char _a5[0x0C];
    s32 fB4;
    s16 fB8;
    char _a6[2];
    SubB sub;
} A;

extern s16 D_801E6E74;
extern s16 D_8019C9AC;
extern u8 D_801E4369;
extern s32 D_8019CACC;
extern u16 D_801E436A;
extern s16 D_801E4B60;
extern s16 D_801E4B62;
extern s16 D_801E437E;
extern s16 D_801E418C;
extern s16 D_8007C020[];

s32 func_8002A7C4(s32, s32);
s32 func_80068634(s32);

void func_8002CD4C(A *ctx) {
    SubB *p = &ctx->sub;
    s16 mode = D_801E6E74;
    s32 v1, a1;
    s32 a0v, r, s2v;

    if (mode < 2) {
        ctx->sub.x1C = 0;
        ctx->f44 = 0;
        goto L_53C;
    }
    if (mode >= 4) goto L_448;
    if (D_8019C9AC != 0) goto L_448;
    if (D_801E4369 != 0x41) goto L_1C4;

    if (D_8019CACC != 0) {
        a1 = D_801E436A & D_801E4B60;
        v1 = D_801E436A & D_801E4B62;
    } else {
        v1 = D_801E436A & D_801E4B60;
        a1 = D_801E436A & D_801E4B62;
    }

    if (v1 == 0) goto L_11c;
    a0v = 2;
    if (ctx->fB8 != 0) a0v = 1;
    v1 = p->x1C;
    p->x40 = a0v;
    if (v1 > 0) { p->x1C = 0; goto L108; }
    if (v1 < -4095) goto L108;
    p->x1C = v1 - 1536;
L108:
    ctx->f64 = ctx->f64 - 6;
    goto L_194;

L_11c:
    if (a1 == 0) goto L_174;
    a0v = 1;
    if (ctx->fB8 != 0) a0v = 2;
    v1 = p->x1C;
    p->x40 = a0v;
    if (v1 < 0) { p->x1C = 0; goto L160; }
    if (v1 >= 4096) goto L160;
    p->x1C = v1 + 1536;
L160:
    ctx->f64 = ctx->f64 + 6;
    goto L_194;

L_174:
    p->x40 = 0;
    p->x1C = p->x1C / 3;

L_194:
    ctx->f44 = -p->x1C;
    if (ctx->f64 != 0) {
        ctx->f64 = (ctx->f64 * 7) / 8;
    }
    goto L_53C;

L_1C4:
    if (D_801E4369 != 0x23) goto L_43C;
    a1 = ((s32)(D_801E437E * 13) << 9) / D_8007C020[D_801E418C];
    if (D_8019CACC != 0) a1 = -a1;
    if (a1 >= 0) goto L_310;

    /* a1 < 0 */
    a0v = 2;
    if (ctx->fB8 != 0) a0v = 1;
    v1 = ctx->sub.x1C;
    ctx->sub.x40 = a0v;
    if (v1 > 0) {
        ctx->sub.x1C = 0;
        ctx->f44 = 0;
        goto L_2FC;
    }
    if ((a1 - 256) < v1) {
        s32 t;
        if (v1 >= 4097) v1 = 4096;
        t = func_80068634(v1 / 8);
        p->x1C = p->x1C - (t / 4);
        ctx->f44 = ctx->f44 + 1536;
        goto L_2FC;
    }
    ctx->sub.x1C = v1 / 3;
L_2FC:
    ctx->f64 = ctx->f64 - 6;
    goto L_414;

L_310:
    if (a1 <= 0) goto L_3E0;
    a0v = 1;
    if (ctx->fB8 != 0) a0v = 2;
    v1 = ctx->sub.x1C;
    ctx->sub.x40 = a0v;
    if (v1 < 0) {
        ctx->sub.x1C = 0;
        ctx->f44 = 0;
        goto L_3CC;
    }
    if (v1 < (a1 + 256)) {
        s32 t;
        s32 c = v1;
        if (v1 < -4096) { v1 = -4096; c = v1; }
        t = func_80068634(c / 8);
        p->x1C = p->x1C + (t / 4);
        ctx->f44 = ctx->f44 - 1536;
        goto L_3CC;
    }
    ctx->sub.x1C = v1 / 3;
L_3CC:
    ctx->f64 = ctx->f64 + 6;
    goto L_414;

L_3E0:
    ctx->sub.x40 = 0;
    ctx->f44 = ctx->f44 / 2;
    ctx->sub.x1C = ctx->sub.x1C / 6;

L_414:
    if (ctx->f64 != 0) {
        ctx->f64 = (ctx->f64 * 7) / 8;
    }
    goto L_53C;

L_43C:
    ctx->f64 = 0;
    ctx->sub.x1C = 0;
    goto L_53C;

L_448:
    {
        s32 arg1 = (ctx->fB8 << 11) + 3072;
        r = func_8002A7C4(ctx->f24, arg1 - ctx->fB4);
    }
    s2v = r * 32;
    r = func_80068634(ctx->f34 * 2);
    a0v = 4096 - r;
    if (ctx->fA4 < 800) {
        a0v = a0v * 6;
    } else {
        a0v = a0v * 4;
    }
    if (ctx->fA4 >= 81) {
        if (ctx->fB8 != 0) {
            if (ctx->f34 < 0) a0v = -a0v;
        } else {
            if (ctx->f34 > 0) a0v = -a0v;
        }
        a0v = a0v + s2v;
    } else {
        p->x1C = 0;
        a0v = 0;
    }
    if (a0v < -4096) a0v = -4096;
    if (a0v > 4096) a0v = 4096;
    p->x1C = a0v;
    ctx->f44 = a0v;
    ctx->f64 = a0v / 128;

L_53C:
    v1 = ctx->fA4;
    if (v1 < 800) {
        s32 f = ctx->f64;
        ctx->f64 = (f * v1) / 800;
    }
}
