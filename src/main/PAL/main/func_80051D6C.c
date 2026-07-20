#include "common.h"
#include "psyq/gte.h"

typedef struct { s32 a, b, c, d; } Vec16;
typedef struct { s32 f0, f1, f2, f3, f4, f5, f6; } Poly;

extern Vec16 D_80011AC4;
extern Vec16 D_80082D6C;
extern s32 D_8009B34C;
extern s32 D_8009B350;
extern s32 D_8009B358;
extern s32 D_8009B35C;
extern s32 D_8009B37C;
extern s32 D_8009B380;
extern s32 D_8009E694;
extern s32 D_8019CB0C;
extern s32 D_801E40E4;

void func_8001A610(void);
void func_80069728(void *a, void *b);
void *func_80069568(void *matrix, void *inout);
void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A4C0(Matrix *mtx, s32 angle);
s32 func_80068568(s32 arg0);
void func_80017794(void *a, void *b, void *c);
void func_800296B4(void *a, s32 b);

void func_80051D6C(void) {
    Matrix mtxA;
    Matrix mtxB;
    Poly poly;
    Vec16 vcopy;
    s32 s1;
    s32 s0;
    s32 s2;

    vcopy = D_80011AC4;

    *(s32 *)0x1F80000C = -64;
    *(s32 *)0x1F800010 = -256;
    *(s32 *)0x1F800008 = 0;
    *(s32 *)0x1F800018 = 0;
    *(s32 *)0x1F80001C = -104;
    *(s32 *)0x1F800020 = 0;

    func_8001A610();
    func_80069728((Matrix *)0x1F800028, &D_80082D6C);

    if (249999 < D_8009B35C) {
        if (D_8009B358 < 2500) {
            D_8009B358 = 2500;
        }
    }

    s1 = D_8009B350 - D_8009B34C;
    if (s1 != 0) {
        if (s1 > 0) {
            s1 = (s1 + 16) / 16;
        } else {
            s1 = (s1 - 16) / 16;
        }
    }

    {
        s32 t = D_8009B34C + s1;
        D_8009B34C = t;
        if (t <= 3071999) {
            s32 a = D_8009B380;
            if (a >= 0) {
                D_8009B34C = t - 2048000;
                D_8009B37C = a;
                D_8009B380 = -1;
            }
        }
    }

    s1 = D_8009B34C / 1000;

    s0 = D_8009B35C - D_8009B358;
    if (s0 != 0) {
        if (s0 > 0) {
            s0 = (250008 - s0) / 8;
        } else {
            s0 = (s0 - 12) / 12;
        }
    }

    D_8009B358 = s0 + D_8009B358;
    s0 = D_8009B358 / 1000;

    s2 = 40;
    if (D_8019CB0C != 0) {
        s2 = 64;
    }

    poly.f0 = 0;
    poly.f1 = (s0 - s2) + func_80068568((D_8009E694 * 32) & 0xFE0) * 12 / 4096;
    poly.f2 = 0;
    poly.f4 = 0;
    poly.f5 = s1;
    poly.f6 = func_80068568((D_8009E694 * 20) & 0xFFC) * 72 / 4096;

    func_8001A530(&mtxB, 0x800 - poly.f5);
    func_8001A4C0(&mtxA, poly.f6);
    func_80069568(&mtxB, &mtxA);
    func_80069568((Matrix *)0x1F800028, &mtxA);
    func_80069728(&mtxA, &vcopy);

    if (D_8009B37C != 10 && (u32)(D_8009B37C - 42) >= 2) {
        s32 a1;
        func_80017794((void *)0x1F80011C, &poly, &mtxA);
        *(s32 *)0x1F800084 = 0;
        a1 = 1;
        if (D_8009B37C < D_801E40E4) {
            a1 = D_8009B37C;
        }
        func_800296B4((void *)0x1F800000, a1);
    }
}
