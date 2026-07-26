#include "common.h"
#include "psyq/gte.h"

s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

void func_8001A4C0(Matrix *mtx, s32 angle) {
    s32 s;
    s32 c;

    s = func_80068568(angle);
    c = func_80068634(angle);
    mtx->m[0][0] = c;
    mtx->m[0][1] = -s;
    mtx->m[0][2] = 0;
    mtx->m[1][0] = s;
    mtx->m[1][1] = c;
    mtx->m[1][2] = 0;
    mtx->m[2][0] = 0;
    mtx->m[2][1] = 0;
    mtx->m[2][2] = 0x1000;
}

s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

void func_8001A530(Matrix *mtx, s32 angle) {
    s32 s;
    s32 c;

    s = func_80068568(angle);
    c = func_80068634(angle);
    mtx->m[0][0] = c;
    mtx->m[0][1] = 0;
    mtx->m[0][2] = -s;
    mtx->m[1][0] = 0;
    mtx->m[1][1] = 0x1000;
    mtx->m[1][2] = 0;
    mtx->m[2][0] = s;
    mtx->m[2][1] = 0;
    mtx->m[2][2] = c;
}

s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

void func_8001A5A0(Matrix *mtx, s32 angle) {
    s32 s;
    s32 c;

    s = func_80068568(angle);
    c = func_80068634(angle);
    mtx->m[0][0] = 0x1000;
    mtx->m[0][1] = 0;
    mtx->m[0][2] = 0;
    mtx->m[1][0] = 0;
    mtx->m[1][1] = c;
    mtx->m[1][2] = -s;
    mtx->m[2][0] = 0;
    mtx->m[2][1] = s;
    mtx->m[2][2] = c;
}

extern Matrix D_8019CB18;

void func_8001A4C0(Matrix *mtx, s32 angle);
void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_80068B98(Matrix *lhs, Matrix *rhs, Matrix *out);
void func_80069568(Matrix *lhs, Matrix *rhs);
void func_80069858(Matrix *mtx);

void func_8001A610(void) {
    Matrix mtx;
    Matrix *scratch = (Matrix *)0x1F800028;

    func_8001A530(scratch, *(s32 *)0x1F80001C);
    func_8001A5A0(&mtx, *(s32 *)0x1F800018);
    func_80069568(&mtx, scratch);
    func_8001A4C0(&mtx, *(s32 *)0x1F800020);
    func_80069568(&mtx, scratch);
    func_8001A530(&mtx, 0x800);
    func_80068B98(&mtx, scratch, &D_8019CB18);
    func_80069858(scratch);
}

extern s16 D_8007B664[];

s32 func_8001A6AC(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0 == 0) {
        if (arg1 == 0) {
            return 0;
        }
        if (arg1 > 0) {
            return 0x400;
        }
        return -0x400;
    }

    if (arg0 > 0) {
        if (arg1 >= 0) {
            if (arg0 < arg1) {
                temp_v1 = D_8007B664[(arg0 << 10) / arg1];
                return 0x400 - temp_v1;
            }
            return D_8007B664[(arg1 << 10) / arg0];
        }

        arg1 = -arg1;
        if (arg0 < arg1) {
            return D_8007B664[(arg0 << 10) / arg1] - 0x400;
        }
        return -D_8007B664[(arg1 << 10) / arg0];
    }

    arg0 = -arg0;
    if (arg1 >= 0) {
        if (arg0 < arg1) {
            return D_8007B664[(arg0 << 10) / arg1] + 0x400;
        }
        temp_v1 = D_8007B664[(arg1 << 10) / arg0];
        return 0x800 - temp_v1;
    }

    arg1 = -arg1;
    if (arg0 < arg1) {
        temp_v1 = D_8007B664[(arg0 << 10) / arg1];
        temp_v0 = 0xC00;
        return temp_v0 - temp_v1;
    }
    return D_8007B664[(arg1 << 10) / arg0] + 0x800;
}
