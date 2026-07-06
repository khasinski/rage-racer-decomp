#include "common.h"
#include "game/render.h"

void func_80046188(GameRenderAxisMatrix *out, s32 arg1, s32 arg2, s32 mode) {
    register s32 tmp asm("$2");

    switch ((mode & 0xFF) - 0x58) {
    case 0:
    case 32:
        out->m[0][0] = 0x1000;
        asm("");
        tmp = -arg1;
        out->m[0][1] = 0;
        out->m[0][2] = 0;
        out->m[1][0] = 0;
        out->m[1][1] = arg2;
        out->m[1][2] = tmp;
        out->m[2][0] = 0;
        out->m[2][1] = arg1;
        out->m[2][2] = arg2;
        break;

    case 1:
    case 33:
        out->m[1][1] = 0x1000;
        asm("");
        tmp = -arg1;
        out->m[0][0] = arg2;
        out->m[0][1] = 0;
        out->m[0][2] = arg1;
        out->m[1][0] = 0;
        out->m[1][2] = 0;
        out->m[2][0] = tmp;
        out->m[2][1] = 0;
        out->m[2][2] = arg2;
        break;

    case 2:
    case 34:
        out->m[0][1] = -arg1;
        out->m[0][0] = arg2;
        out->m[0][2] = 0;
        out->m[1][0] = arg1;
        out->m[1][1] = arg2;
        out->m[1][2] = 0;
        out->m[2][0] = 0;
        out->m[2][1] = 0;
        out->m[2][2] = 0x1000;
        break;
    }
}
