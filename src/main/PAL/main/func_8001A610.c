#include "common.h"
#include "psyq/gte.h"

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
