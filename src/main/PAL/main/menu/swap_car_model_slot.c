#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/track.h"
#include "game/vector.h"
#include "psyq/gte.h"

extern u32 g_CarModelSlot;

/* Flips the double-buffered showroom slot and re-registers it. */
void SwapCarModelSlot(void) {
    g_CarModelSlot = g_CarModelSlot < 1;
    InstallCarModelSlot();
}


void DrawCarSlotHighlight(s32 slot) {
    s32 *scratch = &SCRATCH_PRIM_CURSOR_WORD;
    u8 *base = g_DrawBuffer;
    s32 value = *scratch;

    *scratch = (s32)GameQueueTileTrans(base + 0xCC, (u8 *)value, 0x24, (slot * 16) + 0x24, 0x50, 0x10, 0, 0, 0xFF);
}

typedef struct SwObj698 { s32 unk0; u16 unk4; } SwObj698;
typedef struct SwModelPose {
    s32 position[4];
    s32 unk10[4];
    Vec4 rotation;
} SwModelPose;

extern SwObj698 *g_CarModelAsset;
extern SwModelPose D_8009E6D4 asm("g_PlayerCar");

s32 GetCarAssetIndex(s32 model, s32 grade);
void *ApplyMatrixLV(void *m, void *v0, void *v1);
void DrawPlayerCarModel(void *obj);

void DrawMenuCarView(void) {
    Matrix mtxA;
    Matrix mtxB;
    Vec4 out;
    Vec4 vec;
    s32 s1, s2, s3;
    s32 x;
    s32 targetAngle;
    s32 offset;
    s32 outX;
    s32 altLayout;
    s32 result;
    s32 outZ;
    s32 qValue;
    s32 modelSlot;
    s32 *p;
    s32 *q;

    vec = D_80011AB4;
    SCRATCH_VIEW_Y = -64;
    SCRATCH_VIEW_Z = -256;
    SCRATCH_VIEW_X = 0;
    SCRATCH_VIEW_ANGLE_X = 0x100;
    SCRATCH_VIEW_ANGLE_Y = 0;
    SCRATCH_VIEW_ANGLE_Z = 0;

    SetCameraRotMatrix();
    ScaleMatrix(SCRATCH_VIEW_MATRIX_GTE, &g_MenuViewScale);

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
                if (g_CarSwapToIndex >= 0) {
                    if (g_AssetLoadState != 0) {
                        return;
                    }
                    SwapCarModelSlot();
                    g_CarSwapFromIndex = g_CarSwapToIndex;
                    g_CarSwapToIndex = -1;
                } else {
                    g_MenuViewAngle = (s3 - 24) / 24 + x;
                }
            } else {
                g_MenuViewAngle = (s3 - 24) / 24 + x;
            }
        } else {
            if (x > 900000 && g_CarSwapToIndex >= 0) {
                if (g_AssetLoadState != 0) {
                    return;
                }
                SwapCarModelSlot();
                g_CarSwapFromIndex = g_CarSwapToIndex;
                g_CarSwapToIndex = -1;
            } else {
                g_MenuViewAngle = (s3 + 24) / 24 + x;
            }
        }
    }

    s3 = ((g_MenuViewAngle + 300000) % 600000 - 300000) / 1000;
    s1 = g_CarSwapFromIndex;
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
    D_8009E782 = GetCarAssetIndex(s1, g_CarTable[s1].modelVariant);
    D_8009E7B8 = g_CarTable[s1].tireCompound;

    if (g_PadHeld & 2) {
        if (D_8009E718 < 6144) {
            D_8009E718 = D_8009E718 + 192;
        }
    }
    if (g_PadHeld % 2) {
        s32 *w = &D_8009E718;
        if (*w >= -6143) {
            *w = *w - 192;
        }
    }

    D_8009E804 = g_CarTable[s1].transmission;
    D_8009E71C = (D_8009E71C + 68) & 0xFFF;

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

    p = &D_8009E6D4.rotation.y;
    *p = *p + g_MenuViewSpin;
    BuildRotMatrixY(&mtxA, *p);
    vec.z = (s16)(-((s16)g_CarModelAsset->unk4 / 2));
    ApplyMatrixLV(&mtxA, &vec, &out);
    BuildRotMatrixY(&mtxB, 0x800 - *p);
    BuildRotMatrixX(&mtxA, D_8009E6D4.rotation.x);
    MulMatrix2(&mtxB, &mtxA);
    MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &mtxA);

    altLayout = g_MenuAltLayout;
    outX = out.x;
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
    outZ = out.z;
    qValue = s2 + 30;
    *q = qValue;
    D_8009E6D4.position[2] = -outZ;
    D_8009E724 = D_8009E6D4.rotation;
    D_8009E734 = *q;
    SelectModelBank(modelSlot);
    q--;
    DrawPlayerCarModel(q);

    *q = (g_MenuAltLayout != 0 ? 23 : 52) - s3;
    q = &D_8009E6D4.position[1];
    *q = s2 + 30;
    D_8009E6D4.position[2] = 0;
    SelectModelBank(14);
    D_1F800004 += 120;
    SetGteObjectMatrix((void *)0x1F80011C, &D_8009E6D4.position[0], &mtxA);
    SCRATCH_ENV_MODE4 = 0;
    {
        s32 a1 = 1;
        if (g_ModelBankCount >= 6) {
            a1 = 5;
        }
        SubmitModel((void *)SCRATCHPAD_ADDR, a1);
    }
    D_1F800004 -= 120;
}

extern s32 g_PlayerCar;

/* The course diorama behind COURSE SELECT and RANKING, with the carousel easing. */
void DrawMenuCourseView(void) {
    Matrix mtxA;
    Matrix mtxB;
    s32 s1;
    s32 s0;
    s32 s2;
    s32 *p;

    SCRATCH_VIEW_Y = -64;
    SCRATCH_VIEW_Z = -256;
    SCRATCH_VIEW_X = 0;
    SCRATCH_VIEW_ANGLE_X = 0x100;
    SCRATCH_VIEW_ANGLE_Y = 0;
    SCRATCH_VIEW_ANGLE_Z = 0;

    SetCameraRotMatrix();
    ScaleMatrix(SCRATCH_VIEW_MATRIX_GTE, &g_MenuViewScale);

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
    g_PlayerCarY = s0 + 15;

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
    BuildRotMatrixY(&mtxB, 0x800 - *p);
    BuildRotMatrixX(&mtxA, D_8009E6F4);
    MulMatrix2(&mtxB, &mtxA);
    MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &mtxA);
    SelectModelBank(14);
    SetGteObjectMatrix((void *)0x1F80011C, p - 9, &mtxA);
    SCRATCH_ENV_MODE4 = 0;
    {
        s32 a1 = 1;
        if ((s2 & 3) < g_ModelBankCount) {
            a1 = s2 & 3;
        }
        SubmitModel((void *)SCRATCHPAD_ADDR, a1);
    }
}

typedef struct Poly { s32 f0, f1, f2, f3, f4, f5, f6; } Poly;

s32 rsin(s32 angle);

/* The 3D character model under the TEAM NAME grid cursor; skips the BS and ED cells. */
void DrawTeamNameCharModel(void) {
    Matrix mtxA;
    Matrix mtxB;
    Poly poly;
    Vec4 vcopy;
    s32 s1;
    s32 s0;
    s32 s2;

    vcopy = D_80011AC4;

    SCRATCH_VIEW_Y = -64;
    SCRATCH_VIEW_Z = -256;
    SCRATCH_VIEW_X = 0;
    SCRATCH_VIEW_ANGLE_X = 0;
    SCRATCH_VIEW_ANGLE_Y = -104;
    SCRATCH_VIEW_ANGLE_Z = 0;

    SetCameraRotMatrix();
    ScaleMatrix(SCRATCH_VIEW_MATRIX_GTE, &g_MenuViewScale);

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
    poly.f1 = (s0 - s2) + rsin((g_AnimTimer * 32) & 0xFE0) * 12 / 4096;
    poly.f2 = 0;
    poly.f4 = 0;
    poly.f5 = s1;
    poly.f6 = rsin((g_AnimTimer * 20) & 0xFFC) * 72 / 4096;

    BuildRotMatrixY(&mtxB, 0x800 - poly.f5);
    BuildRotMatrixZ(&mtxA, poly.f6);
    MulMatrix2(&mtxB, &mtxA);
    MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &mtxA);
    ScaleMatrix(&mtxA, &vcopy);

    if (g_TeamNameCharModel != 10 && (u32)(g_TeamNameCharModel - 42) >= 2) {
        s32 a1;
        SetGteObjectMatrix((void *)0x1F80011C, &poly, &mtxA);
        SCRATCH_ENV_MODE4 = 0;
        a1 = 1;
        if (g_TeamNameCharModel < g_CourseModelCount) {
            a1 = g_TeamNameCharModel;
        }
        SubmitCourseModel((void *)SCRATCHPAD_ADDR, a1);
    }
}

void DrawCarSlotLabel(s32 x, s32 y, s32 label) { DrawText8x8(x, y, D_80082E3C[label]); }
