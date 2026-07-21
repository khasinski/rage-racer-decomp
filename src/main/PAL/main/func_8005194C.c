#include "common.h"
#include "psyq/gte.h"

typedef struct { s32 a, b, c, d; } Vec16;

extern Vec16 D_80082D6C;
extern s32 D_8009B34C;
extern s32 D_8009B350;
extern s32 D_8009B354;
extern s32 D_8009B358;
extern s32 D_8009B35C;
extern s32 D_8009B36C;
extern s32 D_8009B370;
extern s32 D_8009E6D4;
extern s32 D_8009E6D8;
extern s32 D_8009E6DC;
extern s32 D_8009E6F4;
extern s32 D_8009E6F8;
extern u16 D_801E436A;
extern s32 D_801E4168;
extern s32 D_801E8268;

void func_8001A610(void);
void func_80069728(void *a, void *b);
void func_8001A530(void *mtx, s32 angle);
void func_8001A5A0(void *mtx, s32 angle);
void *func_80069568(void *a, void *b);
void func_80017A10(s32 a);
void func_80017794(void *a, void *b, void *c);
void func_80028DEC(void *a, s32 b);

void func_8005194C(void) {
    Matrix mtxA;
    Matrix mtxB;
    s32 s1;
    s32 s0;
    s32 s2;
    s32 *p;

    *(s32 *)0x1F80000C = -64;
    *(s32 *)0x1F800010 = -256;
    *(s32 *)0x1F800008 = 0;
    *(s32 *)0x1F800018 = 0x100;
    *(s32 *)0x1F80001C = 0;
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
            if (D_8009B34C > 750000 && D_8009B370 >= 0) {
                if (D_8009B354 >= 19) {
                    D_8009B354 = 0;
                    D_8009B36C = D_8009B370;
                    D_8009B370 = -1;
                } else {
                    D_8009B354 = D_8009B354 + 1;
                }
            } else {
                D_8009B34C = (s1 + 18) / 18 + D_8009B34C;
            }
        } else {
            if (D_8009B34C <= 249999 && D_8009B370 >= 0) {
                if (D_8009B354 >= 19) {
                    D_8009B354 = 0;
                    D_8009B36C = D_8009B370;
                    D_8009B370 = -1;
                } else {
                    D_8009B354 = D_8009B354 + 1;
                }
            } else {
                D_8009B34C = (s1 - 18) / 18 + D_8009B34C;
            }
        }
    }

    s1 = ((D_8009B34C + 250000) % 500000 - 250000) / 1000;

    s2 = D_8009B36C;

    s0 = D_8009B35C - D_8009B358;
    if (s0 != 0) {
        if (s0 > 0) {
            s0 = (250008 - s0) / 8;
        } else {
            s0 = (s0 - 12) / 12;
        }
    }

    D_8009E6D4 = 23 - s1;
    D_8009B358 = s0 + D_8009B358;
    D_8009E6DC = -20;
    s0 = D_8009B358 / 1000;
    D_8009E6D8 = s0 + 15;

    if (D_801E436A & 4) {
        if (D_801E8268 < 64) {
            D_801E8268 = D_801E8268 + 1;
        }
    }
    if (D_801E436A & 8) {
        if (D_801E8268 >= -63) {
            D_801E8268 = D_801E8268 - 1;
        }
    }

    p = &D_8009E6F8;
    *p = *p + D_801E8268;
    func_8001A530(&mtxB, 0x800 - *p);
    func_8001A5A0(&mtxA, D_8009E6F4);
    func_80069568(&mtxB, &mtxA);
    func_80069568((Matrix *)0x1F800028, &mtxA);
    func_80017A10(14);
    func_80017794((void *)0x1F80011C, p - 9, &mtxA);
    *(s32 *)0x1F800084 = 0;
    {
        s32 a1 = 1;
        if ((s2 & 3) < D_801E4168) {
            a1 = s2 & 3;
        }
        func_80028DEC((void *)0x1F800000, a1);
    }
}
