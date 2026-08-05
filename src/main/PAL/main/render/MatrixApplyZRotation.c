#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

s32 rsin(s32 arg0);
s32 rcos(s32 arg0);
void MatrixApplyZRotation(Matrix *arg0, s32 arg1) {
    Matrix sp10;
    s32 angle;
    s32 c;
    s32 s;
    s16 s_copy;

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
        MulMatrix(arg0, &sp10);
    }
}
