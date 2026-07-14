#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

extern GameRenderView *D_8009E698;
extern Matrix D_8009E6AC;
extern s32 D_801E42E4;
extern s32 D_801E4168;
extern s32 D_1F800084;

s32 func_800350B4(s32 arg0);
void func_8001C248(s32 arg0, Matrix *arg1);
void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_80069568(Matrix *lhs, Matrix *rhs);
void func_80069458(Matrix *dst, Matrix *src);
void func_80068B98(Matrix *a, Matrix *b, Matrix *out);
void func_80069888(Matrix *arg0);
s32 *func_80069678(s32 *a, s32 *b, s32 *c);
void func_80017794(void *a0, void *a1, void *a2);
void func_80028DEC(void *arg0, s32 arg1);
void func_8001C794(void);

void func_8001DAB0(GameRenderObject *obj) {
    GameRenderView *view = D_8009E698;
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
    s32 s5;
    s32 s4;
    s32 i;

    obj->y -= view->horizon_6;
    obj->field_60 -= view->horizon_6;
    func_8001A530(&m_10, 0x800 - obj->angle_24);
    func_8001A5A0(&m_30, obj->angle_20);
    func_80069568(&m_10, &m_30);
    func_80068B98(&D_8009E6AC, &m_30, &m_90);

    if (D_801E42E4 != 8) {
        s5 = func_800350B4((s32) obj->field_70);
        if (s5 != 0) {
            func_8001C248(s5, &m_90);
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
    func_80069568((Matrix *)0x1F800028, &m_30);

    func_8001A530(&m_10, 0x800 - obj->angle_54);
    func_8001A5A0(&m_70, obj->angle_50);
    func_80069568(&m_10, &m_70);
    func_80069568((Matrix *)0x1F800028, &m_70);
    func_8001A4C0(&m_10, obj->angle_58);
    func_80069568(&m_70, &m_10);

    v_138[0] = obj->x;
    v_138[2] = obj->z;
    v_138[1] = obj->field_60;
    func_80017794((void *)0x1F80011C, v_138, &m_10);
    D_1F800084 = 0;
    func_80028DEC((void *)0x1F800000, 1);

    func_80017794((void *)0x1F80011C, v_138, &m_10);
    D_1F800084 = 0;
    func_80028DEC((void *)0x1F800000, 1);

    func_8001A4C0(&m_70, obj->angle_28);
    func_80069568(&m_30, &m_70);
    func_80017794((void *)0x1F80011C, obj, &m_70);
    D_1F800084 = 0;
    func_80028DEC((void *)0x1F800000, D_801E4168 < 1);

    s4 = obj->field_E4 << 1;
    if (obj->flags_48 & 0x1000) {
        s4 += 10;
    }
    func_8001A4C0(&m_10, obj->angle_28 - obj->field_64);
    func_80069458(&m_50, &m_10);
    func_80069458(&m_30, &m_10);
    func_8001A5A0(&m_F0, obj->flags_48);
    func_80069568(&m_30, &m_F0);

    func_8001A530(&m_10, obj->angle_44 / 12);
    func_8001A5A0(&m_B0[0], obj->flags_48);
    func_80069568(&m_10, &m_B0[0]);
    func_80069568(&m_30, &m_B0[0]);

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
    func_80028DEC((void *)0x1F800000, (s4 + 3 < D_801E4168) ? (s4 + 3) : 1);

    for (i = 0; i < 2; i++) {
        GameRenderView *v = D_8009E698;
        s32 ax = v->angle_0;
        if (i & 1) {
            ax = -ax;
        }
        v_110[0] = ax;
        v_110[1] = v->angle_2;
        v_110[2] = v->angle_4;
        func_80069678((s32 *)&m_50, (s32 *)v_110, m_118);
        m_118[0] += obj->x;
        m_118[1] += obj->y;
        m_118[2] += obj->z;
        func_80017794((void *)0x1F80011C, m_118, &m_B0[i]);
        D_1F800084 = 0;
        func_80028DEC((void *)0x1F800000, (s4 + 2 < D_801E4168) ? (s4 + 2) : 1);
        func_80069888(&m_90);
    }

    obj->y += D_8009E698->horizon_6;
    obj->field_60 += D_8009E698->horizon_6;
    if (s5 != 0) {
        func_8001C794();
    }
}
