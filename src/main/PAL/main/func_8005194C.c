#include "common.h"
#include "psyq/gte.h"
#include "game/menu.h"
#include "game/render.h"

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
