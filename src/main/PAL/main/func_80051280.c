#include "common.h"
#include "game/render.h"
#include "psyq/gte.h"
#include "game/menu.h"
#include "game/state.h"

extern u32 g_CarModelSlot asm("D_8009E87C");

void func_80051238(void);

void func_80051280(void) {
    g_CarModelSlot = g_CarModelSlot < 1;
    func_80051238();
}

s32 func_8001720C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_800512B4(s32 arg0) {
    s32 *scratch = (s32 *)0x1F800000;
    u8 *base = g_DrawBuffer;
    s32 value = *scratch;

    *scratch = func_8001720C(base + 0xCC, value, 0x24, (arg0 << 4) + 0x24, 0x50, 0x10, 0, 0, 0xFF);
}

/*
 * PERMUTER-CANDIDATE: complete real-C decompilation exists (see
 * scratch/decomp-work/func_8005131C/, ~140 diffs) - not blocked, not
 * handwritten. Residual is a coupled register-allocation/scheduling artifact
 * (the div/diff pseudo wants $s3 but that forces a 5th callee reg + a prologue
 * word, cascading positional diffs). Kept as byte-exact INCLUDE_ASM until a
 * permuter run converges.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_8005131C);

typedef struct { s32 a, b, c, d; } Vec16;

extern Vec16 D_80082D6C;
extern s32 D_8009B354;
extern s32 D_8009B36C;
extern s32 D_8009B370;
extern s32 g_PlayerCar asm("D_8009E6D4");
extern s32 D_8009E6D8;
extern s32 D_8009E6DC;
extern s32 D_8009E6F4;
extern s32 D_8009E6F8;
extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 D_801E8268;

void func_80069728(void *a, void *b);
void func_80017794(void *a, void *b, void *c);

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

    GameSetCameraRotMatrix();
    func_80069728((Matrix *)0x1F800028, &D_80082D6C);

    if (249999 < g_MenuViewOffsetTarget) {
        if (g_MenuViewOffset < 2500) {
            g_MenuViewOffset = 2500;
        }
    }

    s1 = g_MenuViewAngleTarget - g_MenuViewAngle;
    if (s1 != 0) {
        if (s1 > 0) {
            if (g_MenuViewAngle > 750000 && D_8009B370 >= 0) {
                if (D_8009B354 >= 19) {
                    D_8009B354 = 0;
                    D_8009B36C = D_8009B370;
                    D_8009B370 = -1;
                } else {
                    D_8009B354 = D_8009B354 + 1;
                }
            } else {
                g_MenuViewAngle = (s1 + 18) / 18 + g_MenuViewAngle;
            }
        } else {
            if (g_MenuViewAngle <= 249999 && D_8009B370 >= 0) {
                if (D_8009B354 >= 19) {
                    D_8009B354 = 0;
                    D_8009B36C = D_8009B370;
                    D_8009B370 = -1;
                } else {
                    D_8009B354 = D_8009B354 + 1;
                }
            } else {
                g_MenuViewAngle = (s1 - 18) / 18 + g_MenuViewAngle;
            }
        }
    }

    s1 = ((g_MenuViewAngle + 250000) % 500000 - 250000) / 1000;

    s2 = D_8009B36C;

    s0 = g_MenuViewOffsetTarget - g_MenuViewOffset;
    if (s0 != 0) {
        if (s0 > 0) {
            s0 = (250008 - s0) / 8;
        } else {
            s0 = (s0 - 12) / 12;
        }
    }

    g_PlayerCar = 23 - s1;
    g_MenuViewOffset = s0 + g_MenuViewOffset;
    D_8009E6DC = -20;
    s0 = g_MenuViewOffset / 1000;
    D_8009E6D8 = s0 + 15;

    if (g_PadHeld & 4) {
        if (D_801E8268 < 64) {
            D_801E8268 = D_801E8268 + 1;
        }
    }
    if (g_PadHeld & 8) {
        if (D_801E8268 >= -63) {
            D_801E8268 = D_801E8268 - 1;
        }
    }

    p = &D_8009E6F8;
    *p = *p + D_801E8268;
    GameBuildRotMatrixY(&mtxB, 0x800 - *p);
    GameBuildRotMatrixX(&mtxA, D_8009E6F4);
    MulMatrix2(&mtxB, &mtxA);
    MulMatrix2((Matrix *)0x1F800028, &mtxA);
    GameSelectModelBank(14);
    func_80017794((void *)0x1F80011C, p - 9, &mtxA);
    *(s32 *)0x1F800084 = 0;
    {
        s32 a1 = 1;
        if ((s2 & 3) < g_ModelBankCount) {
            a1 = s2 & 3;
        }
        GameSubmitModel((void *)0x1F800000, a1);
    }
}

typedef struct { s32 f0, f1, f2, f3, f4, f5, f6; } Poly;

extern Vec16 D_80011AC4;
extern s32 D_8009B37C;
extern s32 g_CourseModelCount asm("D_801E40E4");
s32 func_80068568(s32 arg0);

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

    GameSetCameraRotMatrix();
    func_80069728((Matrix *)0x1F800028, &D_80082D6C);

    if (249999 < g_MenuViewOffsetTarget) {
        if (g_MenuViewOffset < 2500) {
            g_MenuViewOffset = 2500;
        }
    }

    s1 = g_MenuViewAngleTarget - g_MenuViewAngle;
    if (s1 != 0) {
        if (s1 > 0) {
            s1 = (s1 + 16) / 16;
        } else {
            s1 = (s1 - 16) / 16;
        }
    }

    {
        s32 t = g_MenuViewAngle + s1;
        g_MenuViewAngle = t;
        if (t <= 3071999) {
            s32 a = GameMenuCursorAnim;
            if (a >= 0) {
                g_MenuViewAngle = t - 2048000;
                D_8009B37C = a;
                GameMenuCursorAnim = -1;
            }
        }
    }

    s1 = g_MenuViewAngle / 1000;

    s0 = g_MenuViewOffsetTarget - g_MenuViewOffset;
    if (s0 != 0) {
        if (s0 > 0) {
            s0 = (250008 - s0) / 8;
        } else {
            s0 = (s0 - 12) / 12;
        }
    }

    g_MenuViewOffset = s0 + g_MenuViewOffset;
    s0 = g_MenuViewOffset / 1000;

    s2 = 40;
    if (g_MenuAltLayout != 0) {
        s2 = 64;
    }

    poly.f0 = 0;
    poly.f1 = (s0 - s2) + func_80068568((g_AnimTimer * 32) & 0xFE0) * 12 / 4096;
    poly.f2 = 0;
    poly.f4 = 0;
    poly.f5 = s1;
    poly.f6 = func_80068568((g_AnimTimer * 20) & 0xFFC) * 72 / 4096;

    GameBuildRotMatrixY(&mtxB, 0x800 - poly.f5);
    GameBuildRotMatrixZ(&mtxA, poly.f6);
    MulMatrix2(&mtxB, &mtxA);
    MulMatrix2((Matrix *)0x1F800028, &mtxA);
    func_80069728(&mtxA, &vcopy);

    if (D_8009B37C != 10 && (u32)(D_8009B37C - 42) >= 2) {
        s32 a1;
        func_80017794((void *)0x1F80011C, &poly, &mtxA);
        *(s32 *)0x1F800084 = 0;
        a1 = 1;
        if (D_8009B37C < g_CourseModelCount) {
            a1 = D_8009B37C;
        }
        GameSubmitCourseModel((void *)0x1F800000, a1);
    }
}

extern s32 D_80082E3C[];
void func_80016754(s32 arg0, s32 arg1, s32 arg2);
void func_800520F8(s32 arg0, s32 arg1, s32 arg2) { func_80016754(arg0, arg1, D_80082E3C[arg2]); }
