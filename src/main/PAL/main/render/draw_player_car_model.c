#include "common.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "psyq/gte.h"

extern GameRenderView *g_CarModelAsset;
extern Matrix g_SceneLightMatrix;
extern s32 g_ScratchRenderMode;

s32 GetTrackZoneBlend(s32 position);

/*
 * GameRenderObject -> GPU-primitive submitter. Subtracts the active view's
 * horizon from the object's y, builds a stack of rotation matrices from the
 * object's angle sets, loads each transform into the GTE at 0x1F80011C and
 * dispatches the primitive builder SubmitModel on the scratchpad OT
 * (0x1F800000) at increasing depth buckets. The m_90 negation block and the
 * m_B0[1] block build the mirrored copies (flip X/Z columns). otDepth is the
 * base OT bucket; clipHandle is the optional clip volume from GetTrackZoneBlend.
 */
void DrawPlayerCarModel(GameRenderObject *obj) {
    GameRenderView *view = g_CarModelAsset;
    Matrix m_10;
    Matrix m_30;
    Matrix m_50;
    Matrix m_70;
    Matrix m_90;
    Matrix m_B0[2];
    Matrix m_F0;
    s16 v_110[4];
    s32 m_118[8];
    s32 v_138[3];
    s32 unused_144[4]; /* reserved stack slot present in the original */
    s32 clipHandle;
    s32 otDepth;
    s32 i;

    obj->y -= view->horizon_6;
    obj->field_60 -= view->horizon_6;
    BuildRotMatrixY(&m_10, 0x800 - obj->angleY);
    BuildRotMatrixX(&m_30, obj->angleX);
    MulMatrix2(&m_10, &m_30);
    MulMatrix0(&g_SceneLightMatrix, &m_30, &m_90);

    if (g_SceneId != 8) {
        clipHandle = GetTrackZoneBlend((s32) obj->trackProgress);
        if (clipHandle != 0) {
            ApplyZoneLighting(clipHandle, &m_90);
        }
    }
    SetLightMatrix(&m_90);

    m_90.m[0][0] = -m_90.m[0][0];
    m_90.m[0][2] = -m_90.m[0][2];
    m_90.m[1][0] = -m_90.m[1][0];
    m_90.m[1][2] = -m_90.m[1][2];
    m_90.m[2][0] = -m_90.m[2][0];
    m_90.m[2][2] = -m_90.m[2][2];

    m_50 = m_30;
    MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &m_30);

    BuildRotMatrixY(&m_10, 0x800 - obj->angle_54);
    BuildRotMatrixX(&m_70, obj->angle_50);
    MulMatrix2(&m_10, &m_70);
    MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &m_70);
    BuildRotMatrixZ(&m_10, obj->angle_58);
    MulMatrix2(&m_70, &m_10);

    v_138[0] = obj->x;
    v_138[2] = obj->z;
    v_138[1] = obj->field_60;
    SetGteObjectMatrix((void *)0x1F80011C, v_138, &m_10);
    g_ScratchRenderMode = 0;
    SubmitModel((void *)SCRATCHPAD_ADDR, 1);

    SetGteObjectMatrix((void *)0x1F80011C, v_138, &m_10);
    g_ScratchRenderMode = 0;
    SubmitModel((void *)SCRATCHPAD_ADDR, 1);

    BuildRotMatrixZ(&m_70, obj->angleZ);
    MulMatrix2(&m_30, &m_70);
    SetGteObjectMatrix((void *)0x1F80011C, obj, &m_70);
    g_ScratchRenderMode = 0;
    SubmitModel((void *)SCRATCHPAD_ADDR, g_ModelBankCount < 1);

    otDepth = obj->field_E4 * 2;
    if (obj->flags_48 & 0x1000) {
        otDepth += 10;
    }
    BuildRotMatrixZ(&m_10, obj->angleZ - obj->field_64);
    MulMatrix(&m_50, &m_10);
    MulMatrix(&m_30, &m_10);
    BuildRotMatrixX(&m_F0, obj->flags_48);
    MulMatrix2(&m_30, &m_F0);

    BuildRotMatrixY(&m_10, obj->angle_44 / 12);
    BuildRotMatrixX(&m_B0[0], obj->flags_48);
    MulMatrix2(&m_10, &m_B0[0]);
    MulMatrix2(&m_30, &m_B0[0]);

    m_B0[1].m[0][0] = -m_B0[0].m[0][0];
    m_B0[1].m[0][1] = m_B0[0].m[0][1];
    m_B0[1].m[0][2] = -m_B0[0].m[0][2];
    m_B0[1].m[1][0] = -m_B0[0].m[1][0];
    m_B0[1].m[1][1] = m_B0[0].m[1][1];
    m_B0[1].m[1][2] = -m_B0[0].m[1][2];
    m_B0[1].m[2][0] = -m_B0[0].m[2][0];
    m_B0[1].m[2][1] = m_B0[0].m[2][1];
    m_B0[1].m[2][2] = -m_B0[0].m[2][2];
    SetGteObjectMatrix((void *)0x1F80011C, obj, &m_F0);
    g_ScratchRenderMode = 0;
    SubmitModel((void *)SCRATCHPAD_ADDR, (otDepth + 3 < g_ModelBankCount) ? (otDepth + 3) : 1);

    for (i = 0; i < 2; i++) {
        GameRenderView *v = g_CarModelAsset;
        s32 ax = v->angle_0;
        if (i % 2) {
            ax = -ax;
        }
        v_110[0] = ax;
        v_110[1] = v->angle_2;
        v_110[2] = v->angle_4;
        ApplyMatrix((s32 *)&m_50, (s32 *)v_110, m_118);
        m_118[0] += obj->x;
        m_118[1] += obj->y;
        m_118[2] += obj->z;
        SetGteObjectMatrix((void *)0x1F80011C, m_118, &m_B0[i]);
        g_ScratchRenderMode = 0;
        SubmitModel((void *)SCRATCHPAD_ADDR, (otDepth + 2 < g_ModelBankCount) ? (otDepth + 2) : 1);
        SetLightMatrix(&m_90);
    }

    obj->y += g_CarModelAsset->horizon_6;
    obj->field_60 += g_CarModelAsset->horizon_6;
    if (clipHandle != 0) {
        RestoreColorMatrix();
    }
}

extern u8 *g_CamRow;
void *ApplyMatrixLV(void *mtx, void *vec, void *out);

void DrawCar(GameRenderObject *obj) {
    Matrix m_10;
    Matrix m_30;
    Matrix m_50;
    Matrix m_70;
    Matrix m_90;
    Matrix m_B0[2];
    Matrix m_F0;
    s16 v_110[4];
    s32 m_118[4];
    s32 v_128[4];
    s32 v_138[4];
    s32 v_148[4];
    s32 clipHandle;
    s32 otDepth;
    s32 i;
    s32 model;
    s32 *cam = &SCRATCH_PRIM_CURSOR_WORD;
    s16 *lod;

    model = g_CarModelByCourse[g_CourseIndex][((GameRenderSourcePoint *)obj)->field_AE];
    lod = D_8007D380[model];
    obj->y -= ((CamRow *)(g_CamRow + (model << 3)))->horizon;
    obj->field_60 -= ((CamRow *)(g_CamRow + (model << 3)))->horizon;

    v_128[0] = obj->x - cam[2];
    v_128[1] = 0;
    v_128[2] = obj->z - cam[4];
    ApplyMatrixLV(SCRATCH_VIEW_MATRIX_GTE, v_128, v_148);
    if (v_128[0] < 0) {
        v_128[0] = -v_128[0];
    }
    if (v_128[2] < 0) {
        v_128[2] = -v_128[2];
    }
    otDepth = v_128[0] + v_128[2];
    if (v_148[2] >= 0) {
        if (otDepth < 0xD00) {
            BuildRotMatrixY(&m_10, 0x800 - obj->angleY);
            BuildRotMatrixX(&m_30, obj->angleX);
            MulMatrix2(&m_10, &m_30);
            MulMatrix0(&g_SceneLightMatrix, &m_30, &m_90);
            clipHandle = GetTrackZoneBlend((s32)obj->trackProgress);
            if (clipHandle != 0) {
                ApplyZoneLighting(clipHandle, &m_90);
            }
            SetLightMatrix(&m_90);

            m_90.m[0][0] = -m_90.m[0][0];
            m_90.m[0][2] = -m_90.m[0][2];
            m_90.m[1][0] = -m_90.m[1][0];
            m_90.m[1][2] = -m_90.m[1][2];
            m_90.m[2][0] = -m_90.m[2][0];
            m_90.m[2][2] = -m_90.m[2][2];

            m_50 = m_30;
            MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &m_30);

            BuildRotMatrixY(&m_10, 0x800 - obj->angle_54);
            BuildRotMatrixX(&m_70, obj->angle_50);
            MulMatrix2(&m_10, &m_70);
            MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &m_70);
            BuildRotMatrixZ(&m_10, obj->angle_58);
            MulMatrix2(&m_70, &m_10);

            v_138[0] = obj->x;
            v_138[2] = obj->z;
            v_138[1] = obj->field_60;
            SetGteObjectMatrix((void *)0x1F80011C, v_138, &m_10);
            g_ScratchRenderMode = 0;
            SubmitModel((void *)SCRATCHPAD_ADDR,
                            (lod[0] + 1 < g_ModelBankCount) ? (lod[0] + 1) : 1);

            SetGteObjectMatrix((void *)0x1F80011C, v_138, &m_10);
            g_ScratchRenderMode = 0;
            SubmitModel((void *)SCRATCHPAD_ADDR,
                            (lod[0] + 1 < g_ModelBankCount) ? (lod[0] + 1) : 1);

            BuildRotMatrixZ(&m_70, obj->angleZ);
            MulMatrix2(&m_30, &m_70);
            SetGteObjectMatrix((void *)0x1F80011C, obj, &m_70);
            g_ScratchRenderMode = lod[1] << 16;
            SubmitModel((void *)SCRATCHPAD_ADDR,
                            (lod[0] < g_ModelBankCount) ? lod[0] : 1);

            BuildRotMatrixZ(&m_10, obj->angleZ - obj->field_64);
            MulMatrix(&m_50, &m_10);
            MulMatrix(&m_30, &m_10);
            BuildRotMatrixX(&m_F0, obj->flags_48);
            MulMatrix2(&m_30, &m_F0);

            BuildRotMatrixY(&m_10, obj->angle_44 * 2);
            BuildRotMatrixX(&m_B0[0], obj->flags_48);
            MulMatrix2(&m_10, &m_B0[0]);
            MulMatrix2(&m_30, &m_B0[0]);

            m_B0[1].m[0][0] = -m_B0[0].m[0][0];
            m_B0[1].m[0][1] = m_B0[0].m[0][1];
            m_B0[1].m[0][2] = -m_B0[0].m[0][2];
            m_B0[1].m[1][0] = -m_B0[0].m[1][0];
            m_B0[1].m[1][1] = m_B0[0].m[1][1];
            m_B0[1].m[1][2] = -m_B0[0].m[1][2];
            m_B0[1].m[2][0] = -m_B0[0].m[2][0];
            m_B0[1].m[2][1] = m_B0[0].m[2][1];
            m_B0[1].m[2][2] = -m_B0[0].m[2][2];
            SetGteObjectMatrix((void *)0x1F80011C, obj, &m_F0);
            g_ScratchRenderMode = 0;
            SubmitModel((void *)SCRATCHPAD_ADDR,
                            (lod[0] + 3 < g_ModelBankCount) ? (lod[0] + 3) : 1);

            for (i = 0; i < 2; i++) {
                s32 ax = ((CamRow *)(g_CamRow + (model << 3)))->axis0;
                if (i % 2) {
                    ax = -ax;
                }
                v_110[0] = ax;
                v_110[1] = ((CamRow *)(g_CamRow + (model << 3)))->axis1;
                v_110[2] = ((CamRow *)(g_CamRow + (model << 3)))->axis2;
                ApplyMatrix((s32 *)&m_50, (s32 *)v_110, m_118);
                m_118[0] += obj->x;
                m_118[1] += obj->y;
                m_118[2] += obj->z;
                SetGteObjectMatrix((void *)0x1F80011C, m_118, &m_B0[i]);
                g_ScratchRenderMode = 0;
                SubmitModel((void *)SCRATCHPAD_ADDR,
                                (lod[0] + 2 < g_ModelBankCount) ? (lod[0] + 2) : 1);
                SetLightMatrix(&m_90);
            }
        } else if (otDepth < 0x2500) {
            BuildRotMatrixY(&m_10, 0x800 - obj->angleY);
            BuildRotMatrixX(&m_50, obj->angleX);
            MulMatrix2(&m_10, &m_50);
            MulMatrix0(&g_SceneLightMatrix, &m_50, &m_90);
            clipHandle = GetTrackZoneBlend((s32)obj->trackProgress);
            if (clipHandle != 0) {
                ApplyZoneLighting(clipHandle, &m_90);
            }
            SetLightMatrix(&m_90);

            BuildRotMatrixZ(&m_10, obj->angleZ);
            MulMatrix2(&m_50, &m_10);
            MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &m_10);
            SetGteObjectMatrix((void *)0x1F80011C, obj, &m_10);
            g_ScratchRenderMode = lod[1] << 16;
            SubmitModel((void *)SCRATCHPAD_ADDR,
                            (lod[0] + 4 < g_ModelBankCount) ? (lod[0] + 4) : 1);
        }
    }

    obj->y += ((CamRow *)(g_CamRow + (model << 3)))->horizon;
    obj->field_60 += ((CamRow *)(g_CamRow + (model << 3)))->horizon;
    if (clipHandle != 0) {
        RestoreColorMatrix();
    }
}
s32 CdControl(s32 com, void *param, s32 result);
