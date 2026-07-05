#include "common.h"
#include "psyq/gte.h"

extern s16 D_80092B08[];
extern s16 D_80093308[];
extern s16 D_80093B08[];
extern s16 D_80094308[];

s32 rsin(s32 arg0) asm("func_80068568");
s32 rsinCore(s32 arg0) asm("func_800685A4");
s32 rcos(s32 arg0) asm("func_80068634");
void func_80069458(Matrix *arg0, Matrix *arg1);

void MatrixApplyZRotation(Matrix *arg0, s32 arg1) asm("func_800684B0");
void MatrixApplyZRotation(Matrix *arg0, s32 arg1) {
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

s32 rsin(s32 arg0) {
    s32 ret;

    if (arg0 < 0) {
        goto negative;
    }

    ret = rsinCore(arg0 & 0xFFF);
    goto done;

negative:
    ret = -rsinCore(-arg0 & 0xFFF);

done:
    return ret;
}

s32 rsinCore(s32 arg0) {
    if (arg0 < 0x801) {
        if (arg0 < 0x401) {
            return D_80094308[arg0];
        }

        return D_80094308[0x800 - arg0];
    }

    if (arg0 < 0xC01) {
        return -D_80093308[arg0];
    }

    return -D_80094308[0x1000 - arg0];
}

s32 rcos(s32 arg0) {
    if (arg0 < 0) {
        arg0 = -arg0;
    }

    arg0 &= 0xFFF;

    if (arg0 < 0x801) {
        if (arg0 < 0x401) {
            return D_80094308[0x400 - arg0];
        }

        return -D_80093B08[arg0];
    }

    if (arg0 < 0xC01) {
        return -D_80094308[0xC00 - arg0];
    }

    return D_80092B08[arg0];
}
