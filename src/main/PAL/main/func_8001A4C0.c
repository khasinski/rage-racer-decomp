#include "common.h"
#include "psyq/gte.h"

s32 rsin(s32 arg0) asm("func_80068568");
s32 rcos(s32 arg0) asm("func_80068634");

void func_8001A4C0(Matrix *mtx, s32 angle) {
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
