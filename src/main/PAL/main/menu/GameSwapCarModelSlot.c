#include "common.h"
#include "game/render.h"
#include "psyq/gte.h"
#include "game/menu.h"
#include "game/state.h"

extern u32 g_CarModelSlot asm("D_8009E87C");

void func_80051238(void);

/* Flips the double-buffered showroom slot and re-registers it. */
void GameSwapCarModelSlot(void) asm("func_80051280");
void GameSwapCarModelSlot(void) {
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

typedef struct { s32 a, b, c, d; } SwVec;
typedef struct { s32 unk0; u16 unk4; } SwObj698;
typedef struct { u8 b0, b1, b2, b3, b4, b5, b6, b7; } SwCarRec;
typedef struct {
    s32 position[4];
    s32 unk10[4];
    SwVec rotation;
} SwModelPose;

extern SwVec D_80011AB4;
extern SwVec D_80082D6C;
extern s32 D_8009B374;
extern s32 D_8009B378;
extern s32 D_8007BED8;
extern SwCarRec *D_8019C7C8;
extern SwObj698 *D_8009E698;
extern s16 D_8009E782;
extern s32 D_8009E7B8;
extern s32 D_8009E718;
extern s32 D_8009E71C;
extern s16 D_8009E804;
extern s32 D_801E8268;
extern SwModelPose D_8009E6D4;
extern SwVec D_8009E724;
extern s32 D_8009E734;
extern s32 D_801E4168;
extern s32 D_1F800004;

s32 func_80017848(s32 arg0, s32 arg1);
void *func_80068F80(void *m, void *v0, void *v1);
void func_8001DAB0(void *obj);

void func_8005131C(void) {
    Matrix mtxA;
    Matrix mtxB;
    SwVec out;
    SwVec vec;
    s32 s1, s2, s3;
    s32 x;
    s32 targetAngle;
    s32 offset;
    s32 outX;
    s32 altLayout;
    s32 result;
    register s32 outZ asm("$2");
    register s32 qValue asm("$3");
    s32 modelSlot;
    s32 *p;
    s32 *q;

    vec = D_80011AB4;
    *(s32 *)0x1F80000C = -64;
    *(s32 *)0x1F800010 = -256;
    *(s32 *)0x1F800008 = 0;
    *(s32 *)0x1F800018 = 0x100;
    *(s32 *)0x1F80001C = 0;
    *(s32 *)0x1F800020 = 0;

    GameSetCameraRotMatrix();
    ScaleMatrix((void *)0x1F800028, &D_80082D6C);

    if (249999 < g_MenuViewOffsetTarget) {
        if (g_MenuViewOffset < 2500) {
            g_MenuViewOffset = 2500;
        }
    }

    targetAngle = g_MenuViewAngleTarget;
    x = g_MenuViewAngle;
    s3 = targetAngle - x;
    if (s3 != 0) {
        if (s3 < 0) {
            if (x <= 299999) {
                if (D_8009B378 >= 0) {
                    if (D_8007BED8 != 0) {
                        return;
                    }
                    GameSwapCarModelSlot();
                    D_8009B374 = D_8009B378;
                    D_8009B378 = -1;
                } else {
                    g_MenuViewAngle = (s3 - 24) / 24 + x;
                }
            } else {
                g_MenuViewAngle = (s3 - 24) / 24 + x;
            }
        } else {
            if (x > 900000 && D_8009B378 >= 0) {
                if (D_8007BED8 != 0) {
                    return;
                }
                GameSwapCarModelSlot();
                D_8009B374 = D_8009B378;
                D_8009B378 = -1;
            } else {
                g_MenuViewAngle = (s3 + 24) / 24 + x;
            }
        }
    }

    s3 = ((g_MenuViewAngle + 300000) % 600000 - 300000) / 1000;
    s1 = D_8009B374;
    s2 = g_MenuViewOffsetTarget - g_MenuViewOffset;
    if (s2 != 0) {
        if (s2 > 0) {
            s2 = (250008 - s2) / 8;
        } else {
            s2 = (s2 - 12) / 12;
        }
    }

    g_MenuViewOffset = s2 + g_MenuViewOffset;
    s2 = g_MenuViewOffset / 1000;
    D_8009E782 = func_80017848(s1, D_8019C7C8[s1].b0);
    D_8009E7B8 = D_8019C7C8[s1].b1;

    if (g_PadHeld & 2) {
        if (D_8009E718 < 6144) {
            D_8009E718 = D_8009E718 + 192;
        }
    }
    if (g_PadHeld & 1) {
        s32 *w = &D_8009E718;
        if (*w >= -6143) {
            *w = *w - 192;
        }
    }

    D_8009E804 = D_8019C7C8[s1].b2;
    D_8009E71C = (D_8009E71C + 68) & 0xFFF;

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

    p = &D_8009E6D4.rotation.b;
    *p = *p + D_801E8268;
    GameBuildRotMatrixY(&mtxA, *p);
    vec.c = (s16)(-((s16)D_8009E698->unk4 / 2));
    func_80068F80(&mtxA, &vec, &out);
    GameBuildRotMatrixY(&mtxB, 0x800 - *p);
    GameBuildRotMatrixX(&mtxA, D_8009E6D4.rotation.a);
    MulMatrix2(&mtxB, &mtxA);
    MulMatrix2((void *)0x1F800028, &mtxA);

    altLayout = g_MenuAltLayout;
    outX = out.a;
    asm volatile("" : "=r"(altLayout), "=r"(outX) : "0"(altLayout), "1"(outX));
    p = &D_8009E6D4.position[0];
    if (altLayout != 0) {
        offset = s3 - 23;
    } else {
        offset = s3 - 52;
    }
    result = outX - offset;
    modelSlot = g_CarModelSlot;
    asm volatile("" : "=r"(result), "=r"(modelSlot) : "0"(result), "1"(modelSlot));
    q = &D_8009E6D4.position[1];
    *p = result;
    outZ = out.c;
    qValue = s2 + 30;
    *q = qValue;
    D_8009E6D4.position[2] = -outZ;
    asm volatile("" : : : "memory");
    D_8009E724 = D_8009E6D4.rotation;
    D_8009E734 = *q;
    GameSelectModelBank(modelSlot);
    q--;
    func_8001DAB0(q);

    *q = (g_MenuAltLayout != 0 ? 23 : 52) - s3;
    q = &D_8009E6D4.position[1];
    *q = s2 + 30;
    D_8009E6D4.position[2] = 0;
    GameSelectModelBank(14);
    D_1F800004 += 120;
    GameSetGteObjectMatrix((void *)0x1F80011C, &D_8009E6D4.position[0], &mtxA);
    *(s32 *)0x1F800084 = 0;
    {
        s32 a1 = 1;
        if (D_801E4168 >= 6) {
            a1 = 5;
        }
        GameSubmitModel((void *)0x1F800000, a1);
    }
    D_1F800004 -= 120;
}

typedef struct { s32 a, b, c, d; } Vec16;

extern Vec16 g_MenuViewScale asm("D_80082D6C");
extern s32 g_CourseSwapDelay asm("D_8009B354");
extern s32 g_MenuCourseModelIndex asm("D_8009B36C");
extern s32 g_MenuPendingCourseIndex asm("D_8009B370");
extern s32 g_PlayerCar asm("D_8009E6D4");
extern s32 D_8009E6D8;
extern s32 g_PlayerCarZ asm("D_8009E6DC");
extern s32 D_8009E6F4;
extern s32 D_8009E6F8;
extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 g_MenuViewSpin asm("D_801E8268");

void func_80017794(void *a, void *b, void *c);

/* The course diorama behind COURSE SELECT and RANKING, with the carousel easing. */
void GameDrawMenuCourseView(void) asm("func_8005194C");
void GameDrawMenuCourseView(void) {
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
    ScaleMatrix((Matrix *)0x1F800028, &g_MenuViewScale);

    if (249999 < g_MenuViewOffsetTarget) {
        if (g_MenuViewOffset < 2500) {
            g_MenuViewOffset = 2500;
        }
    }

    s1 = g_MenuViewAngleTarget - g_MenuViewAngle;
    if (s1 != 0) {
        if (s1 > 0) {
            if (g_MenuViewAngle > 750000 && g_MenuPendingCourseIndex >= 0) {
                if (g_CourseSwapDelay >= 19) {
                    g_CourseSwapDelay = 0;
                    g_MenuCourseModelIndex = g_MenuPendingCourseIndex;
                    g_MenuPendingCourseIndex = -1;
                } else {
                    g_CourseSwapDelay = g_CourseSwapDelay + 1;
                }
            } else {
                g_MenuViewAngle = (s1 + 18) / 18 + g_MenuViewAngle;
            }
        } else {
            if (g_MenuViewAngle <= 249999 && g_MenuPendingCourseIndex >= 0) {
                if (g_CourseSwapDelay >= 19) {
                    g_CourseSwapDelay = 0;
                    g_MenuCourseModelIndex = g_MenuPendingCourseIndex;
                    g_MenuPendingCourseIndex = -1;
                } else {
                    g_CourseSwapDelay = g_CourseSwapDelay + 1;
                }
            } else {
                g_MenuViewAngle = (s1 - 18) / 18 + g_MenuViewAngle;
            }
        }
    }

    s1 = ((g_MenuViewAngle + 250000) % 500000 - 250000) / 1000;

    s2 = g_MenuCourseModelIndex;

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
    g_PlayerCarZ = -20;
    s0 = g_MenuViewOffset / 1000;
    D_8009E6D8 = s0 + 15;

    if (g_PadHeld & 4) {
        if (g_MenuViewSpin < 64) {
            g_MenuViewSpin = g_MenuViewSpin + 1;
        }
    }
    if (g_PadHeld & 8) {
        if (g_MenuViewSpin >= -63) {
            g_MenuViewSpin = g_MenuViewSpin - 1;
        }
    }

    p = &D_8009E6F8;
    *p = *p + g_MenuViewSpin;
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
extern s32 g_TeamNameCharModel asm("D_8009B37C");
extern s32 g_CourseModelCount asm("D_801E40E4");
s32 func_80068568(s32 arg0);

/* The 3D character model under the TEAM NAME grid cursor; skips the BS and ED cells. */
void GameDrawTeamNameCharModel(void) asm("func_80051D6C");
void GameDrawTeamNameCharModel(void) {
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
    ScaleMatrix((Matrix *)0x1F800028, &g_MenuViewScale);

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
                g_TeamNameCharModel = a;
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
    ScaleMatrix(&mtxA, &vcopy);

    if (g_TeamNameCharModel != 10 && (u32)(g_TeamNameCharModel - 42) >= 2) {
        s32 a1;
        func_80017794((void *)0x1F80011C, &poly, &mtxA);
        *(s32 *)0x1F800084 = 0;
        a1 = 1;
        if (g_TeamNameCharModel < g_CourseModelCount) {
            a1 = g_TeamNameCharModel;
        }
        GameSubmitCourseModel((void *)0x1F800000, a1);
    }
}

extern s32 D_80082E3C[];
void func_80016754(s32 arg0, s32 arg1, s32 arg2);
void func_800520F8(s32 arg0, s32 arg1, s32 arg2) { func_80016754(arg0, arg1, D_80082E3C[arg2]); }
