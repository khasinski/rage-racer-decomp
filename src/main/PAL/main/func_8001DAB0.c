#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/state.h"

extern GameRenderView *g_CarModelAsset asm("D_8009E698");
extern Matrix g_SceneLightMatrix asm("D_8009E6AC");
extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 D_1F800084;

s32 func_800350B4(s32 arg0);
void func_8001C248(s32 arg0, Matrix *arg1);
void func_80069888(Matrix *arg0);
void func_80017794(void *a0, void *a1, void *a2);
void func_8001C794(void);

/*
 * GameRenderObject -> GPU-primitive submitter. Subtracts the active view's
 * horizon from the object's y, builds a stack of rotation matrices from the
 * object's angle sets, loads each transform into the GTE at 0x1F80011C and
 * dispatches the primitive builder GameSubmitModel on the scratchpad OT
 * (0x1F800000) at increasing depth buckets. The m_90 negation block and the
 * m_B0[1] block build the mirrored copies (flip X/Z columns). otDepth is the
 * base OT bucket; clipHandle is the optional clip volume from func_800350B4.
 */
void func_8001DAB0(GameRenderObject *obj) {
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
    GameBuildRotMatrixY(&m_10, 0x800 - obj->angle_24);
    GameBuildRotMatrixX(&m_30, obj->angle_20);
    MulMatrix2(&m_10, &m_30);
    MulMatrix0(&g_SceneLightMatrix, &m_30, &m_90);

    if (g_SceneId != 8) {
        clipHandle = func_800350B4((s32) obj->field_70);
        if (clipHandle != 0) {
            func_8001C248(clipHandle, &m_90);
        }
    }
    func_80069888(&m_90);

    m_90.m[0][0] = -m_90.m[0][0];
    m_90.m[0][2] = -m_90.m[0][2];
    m_90.m[1][0] = -m_90.m[1][0];
    m_90.m[1][2] = -m_90.m[1][2];
    m_90.m[2][0] = -m_90.m[2][0];
    m_90.m[2][2] = -m_90.m[2][2];

    m_50 = m_30;
    MulMatrix2((Matrix *)0x1F800028, &m_30);

    GameBuildRotMatrixY(&m_10, 0x800 - obj->angle_54);
    GameBuildRotMatrixX(&m_70, obj->angle_50);
    MulMatrix2(&m_10, &m_70);
    MulMatrix2((Matrix *)0x1F800028, &m_70);
    GameBuildRotMatrixZ(&m_10, obj->angle_58);
    MulMatrix2(&m_70, &m_10);

    v_138[0] = obj->x;
    v_138[2] = obj->z;
    v_138[1] = obj->field_60;
    func_80017794((void *)0x1F80011C, v_138, &m_10);
    D_1F800084 = 0;
    GameSubmitModel((void *)0x1F800000, 1);

    func_80017794((void *)0x1F80011C, v_138, &m_10);
    D_1F800084 = 0;
    GameSubmitModel((void *)0x1F800000, 1);

    GameBuildRotMatrixZ(&m_70, obj->angle_28);
    MulMatrix2(&m_30, &m_70);
    func_80017794((void *)0x1F80011C, obj, &m_70);
    D_1F800084 = 0;
    GameSubmitModel((void *)0x1F800000, g_ModelBankCount < 1);

    otDepth = obj->field_E4 << 1;
    if (obj->flags_48 & 0x1000) {
        otDepth += 10;
    }
    GameBuildRotMatrixZ(&m_10, obj->angle_28 - obj->field_64);
    MulMatrix(&m_50, &m_10);
    MulMatrix(&m_30, &m_10);
    GameBuildRotMatrixX(&m_F0, obj->flags_48);
    MulMatrix2(&m_30, &m_F0);

    GameBuildRotMatrixY(&m_10, obj->angle_44 / 12);
    GameBuildRotMatrixX(&m_B0[0], obj->flags_48);
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
    func_80017794((void *)0x1F80011C, obj, &m_F0);
    D_1F800084 = 0;
    GameSubmitModel((void *)0x1F800000, (otDepth + 3 < g_ModelBankCount) ? (otDepth + 3) : 1);

    for (i = 0; i < 2; i++) {
        GameRenderView *v = g_CarModelAsset;
        s32 ax = v->angle_0;
        if (i & 1) {
            ax = -ax;
        }
        v_110[0] = ax;
        v_110[1] = v->angle_2;
        v_110[2] = v->angle_4;
        ApplyMatrix((s32 *)&m_50, (s32 *)v_110, m_118);
        m_118[0] += obj->x;
        m_118[1] += obj->y;
        m_118[2] += obj->z;
        func_80017794((void *)0x1F80011C, m_118, &m_B0[i]);
        D_1F800084 = 0;
        GameSubmitModel((void *)0x1F800000, (otDepth + 2 < g_ModelBankCount) ? (otDepth + 2) : 1);
        func_80069888(&m_90);
    }

    obj->y += g_CarModelAsset->horizon_6;
    obj->field_60 += g_CarModelAsset->horizon_6;
    if (clipHandle != 0) {
        func_8001C794();
    }
}
