#include "common.h"
#include "psyq/gte.h"

s32 rsin(s32 arg0) asm("func_80068568");
s32 rcos(s32 arg0) asm("func_80068634");

void BuildRotMatrixZ(Matrix *mtx, s32 angle) asm("func_8001A4C0");
void BuildRotMatrixY(Matrix *mtx, s32 angle) asm("func_8001A530");
void BuildRotMatrixX(Matrix *mtx, s32 angle) asm("func_8001A5A0");
void SetCameraRotMatrix(void) asm("func_8001A610");
s32 Atan2(s32 x, s32 y) asm("func_8001A6AC");

void BuildRotMatrixZ(Matrix *mtx, s32 angle) {
    s32 s;
    s32 c;

    s = rsin(angle);
    c = rcos(angle);
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

s32 rsin(s32 arg0) asm("func_80068568");
s32 rcos(s32 arg0) asm("func_80068634");

void BuildRotMatrixY(Matrix *mtx, s32 angle) {
    s32 s;
    s32 c;

    s = rsin(angle);
    c = rcos(angle);
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

s32 rsin(s32 arg0) asm("func_80068568");
s32 rcos(s32 arg0) asm("func_80068634");

void BuildRotMatrixX(Matrix *mtx, s32 angle) {
    s32 s;
    s32 c;

    s = rsin(angle);
    c = rcos(angle);
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

void func_80069858(Matrix *mtx);

void SetCameraRotMatrix(void) {
    Matrix mtx;
    Matrix *scratch = (Matrix *)0x1F800028;

    BuildRotMatrixY(scratch, *(s32 *)0x1F80001C);
    BuildRotMatrixX(&mtx, *(s32 *)0x1F800018);
    MulMatrix2(&mtx, scratch);
    BuildRotMatrixZ(&mtx, *(s32 *)0x1F800020);
    MulMatrix2(&mtx, scratch);
    BuildRotMatrixY(&mtx, 0x800);
    MulMatrix0(&mtx, scratch, &D_8019CB18);
    func_80069858(scratch);
}

extern s16 g_AtanTable[] asm("D_8007B664");

s32 Atan2(s32 arg0, s32 arg1) {
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
                return 0x400 - g_AtanTable[(arg0 << 10) / arg1];
            }
            return g_AtanTable[(arg1 << 10) / arg0];
        }

        arg1 = -arg1;
        if (arg0 < arg1) {
            return g_AtanTable[(arg0 << 10) / arg1] - 0x400;
        }
        return -g_AtanTable[(arg1 << 10) / arg0];
    }

    arg0 = -arg0;
    if (arg1 >= 0) {
        if (arg0 < arg1) {
            return g_AtanTable[(arg0 << 10) / arg1] + 0x400;
        }
        return 0x800 - g_AtanTable[(arg1 << 10) / arg0];
    }

    arg1 = -arg1;
    if (arg0 < arg1) {
        return 0xC00 - g_AtanTable[(arg0 << 10) / arg1];
    }
    return g_AtanTable[(arg1 << 10) / arg0] + 0x800;
}
