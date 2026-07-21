#include "common.h"
#include "psyq/gte.h"

extern s16 D_8019CABC;
extern s32 D_801E3EB4;
extern s32 D_801E3EB8;
extern s32 D_801E3EBC;
extern s32 D_801E3ED8;
extern s32 D_801E3EE0;
extern s32 D_8009E6A8;
extern u16 D_801E436A;
extern s16 D_8007E288[];
extern s16 D_8009E74C;
extern s32 D_8009E73C;
extern s32 D_8009E740;
extern s32 D_8009E744;

typedef struct Cam {
    s32 x;        /* 0x00 */
    s32 field_04; /* 0x04 */
    s32 z;        /* 0x08 */
    s32 field_0C; /* 0x0C */
    u8 pad10[0x10];
    s32 field_20; /* 0x20 */
    s32 field_24; /* 0x24 */
    s32 field_28; /* 0x28 */
    s32 field_2C; /* 0x2C */
    s32 field_30; /* 0x30 */
    u8 pad34[4];
    s32 field_38; /* 0x38 */
    u8 pad3C[0x2C];
    s32 field_68; /* 0x68 */
    s32 field_6C; /* 0x6C */
    s32 field_70; /* 0x70 */
    s32 field_74; /* 0x74 */
    u16 field_78; /* 0x78 */
} Cam;

extern Cam D_801E3E14;

s32 func_8001A6AC(s32 arg0, s32 arg1);
void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_8002A7C4(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
s32 func_8006888C(s32 arg0);
void func_8002C168(void *arg0);
void func_80031298(void *arg0, s32 arg1, void *arg2);
void func_8001A610(void);
void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_80069568(Matrix *lhs, Matrix *rhs);
void func_80069678(Matrix *mtx, void *in, void *out);

typedef struct Block16 {
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w3;
} Block16;

void func_8003CF14(s32 arg0, s32 arg1) {
    s32 *scratch = (s32 *)0x1F800000;
    s32 delta[3];
    s32 coords[3];
    s16 pair[2];
    s16 vec[3];
    s32 out[3];
    Matrix m1;
    Matrix m2;
    s32 offset;
    s32 index;
    s32 rem;
    s32 angle;
    s32 value;
    s32 zValue;
    s32 c400;

    offset = D_801E3E14.field_30;
    if (D_8019CABC != 0) {
        index = offset + 2;
    } else {
        index = offset - 2;
    }
    rem = index;
    if (index < 0) {
        rem = index + D_8009E6A8;
    }
    index = rem % D_8009E6A8;

    func_8002FC84(index, coords, D_801E3E14.field_38);
    angle = 0x400 - func_8001A6AC(coords[0] - D_801E3E14.x, coords[2] - D_801E3E14.z);
    D_801E3EB4 += func_8002A7C4(D_801E3EB4, angle);
    D_801E3E14.field_24 = D_801E3EB4;

    if (arg1 != 0) {
        if (D_801E3EBC < 50) {
            D_801E3EBC += 3;
        }
        D_801E3EB8 = (D_801E3EB8 + D_801E3EBC) * 94 / 100;

        value = func_80068568(D_801E3EB4) * D_801E3EB8;
        if (value < 0) {
            value += 0xFF;
        }
        D_801E3ED8 = value >> 8;

        zValue = func_80068634(D_801E3EB4) * D_801E3EB8;
        if (zValue < 0) {
            zValue += 0xFF;
        }
        D_801E3EE0 = zValue >> 8;

        D_801E3E14.x = D_801E3ED8 / 256 + D_801E3E14.x;
        D_801E3E14.z = D_801E3EE0 / 256 + D_801E3E14.z;

        if (D_801E436A & 0x1000) {
            D_8007E288[0] -= 8;
        } else if (D_801E436A & 0x4000) {
            D_8007E288[0] += 8;
        }
        if (D_801E436A & 0x8000) {
            D_8007E288[1] -= 16;
        } else if (D_801E436A & 0x2000) {
            D_8007E288[1] += 16;
        }
    }
    if (D_8007E288[0] >= 0) {
        D_8007E288[0] = 0;
    } else if (D_8007E288[0] < -255) {
        D_8007E288[0] = -256;
    }

    D_801E3E14.field_24 = D_8007E288[1] + D_801E3E14.field_24;
    func_8002C168(&D_801E3E14);
    pair[0] = 0;
    pair[1] = 0;
    func_80031298(&D_801E3E14, D_801E3E14.field_30, pair);

    *(Block16 *)(scratch + 2) = *(Block16 *)&D_801E3E14.x;
    scratch[3] -= 48;
    *(Block16 *)(scratch + 6) = *(Block16 *)&D_801E3E14.field_20;
    scratch[6] = D_8007E288[0] + scratch[6];

    func_8001A530(&m1, scratch[7]);
    func_8001A5A0(&m2, scratch[6]);
    func_80069568(&m2, &m1);
    func_8001A4C0(&m2, scratch[8]);
    func_80069568(&m2, &m1);

    vec[0] = 0;
    vec[1] = 0;
    vec[2] = 0x18C0;
    m2.m[0][0] = m1.m[0][0];
    m2.m[0][1] = m1.m[1][0];
    m2.m[0][2] = m1.m[2][0];
    m2.m[1][0] = m1.m[0][1];
    m2.m[1][1] = m1.m[1][1];
    m2.m[1][2] = m1.m[2][1];
    m2.m[2][0] = m1.m[0][2];
    m2.m[2][1] = m1.m[1][2];
    m2.m[2][2] = m1.m[2][2];
    func_80069678(&m2, vec, out);

    coords[0] = (out[0] >> 4) + scratch[2];
    coords[1] = (out[1] >> 4) + scratch[3];
    coords[2] = (out[2] >> 4) + scratch[4];
    delta[0] = coords[0] - scratch[2];
    delta[1] = coords[1] - scratch[3];
    delta[2] = coords[2] - scratch[4];
    c400 = 0x400;
    __asm__("" : "=r"(c400) : "0"(c400));
    scratch[7] = c400 - func_8001A6AC(delta[0], delta[2]);
    value = func_8006888C(delta[0] * delta[0] + delta[2] * delta[2]);
    scratch[6] = c400 - func_8001A6AC(delta[1], value >> 6);

    D_8009E74C = D_801E3E14.field_78;
    D_8009E73C = D_801E3E14.field_68;
    D_8009E740 = D_801E3E14.field_6C;
    D_8009E744 = D_801E3E14.field_70;
    func_8001A610();
}
