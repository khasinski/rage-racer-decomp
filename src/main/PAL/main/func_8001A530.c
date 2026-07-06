#include "common.h"
#include "psyq/gte.h"

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
