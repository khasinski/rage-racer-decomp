#include "common.h"

typedef struct Matrix {
    s16 m[3][3];
    s32 t[3];
} Matrix;

s32 rsin(s32 arg0) asm("func_80068568");
s32 rcos(s32 arg0) asm("func_80068634");
void func_80069458(Matrix *arg0, Matrix *arg1);

void func_800684B0(Matrix *arg0, s32 arg1) {
    Matrix sp10;
    register s32 angle asm("$16");
    s32 c;
    register s32 s asm("$2");
    register s32 s_copy asm("$3");

    angle = arg1 / 360;
    c = rcos(angle);
    s = rsin(angle);
    s_copy = s;

    if (arg1 != 0) {
        sp10.m[0][0] = c;
        sp10.m[0][1] = -s;
        sp10.m[0][2] = 0;
        sp10.m[1][0] = s_copy;
        sp10.m[1][1] = c;
        sp10.m[1][2] = 0;
        sp10.m[2][0] = 0;
        sp10.m[2][1] = 0;
        sp10.m[2][2] = 0x1000;
        sp10.t[0] = 0;
        sp10.t[1] = 0;
        sp10.t[2] = 0;
        func_80069458(arg0, &sp10);
    }
}
