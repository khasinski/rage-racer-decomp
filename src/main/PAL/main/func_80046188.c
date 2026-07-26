#include "common.h"
#include "game/render.h"

/*
 * Builds a single-axis rotation matrix into `out` (0x1000 == 1.0 fixed-point).
 * axisMode selects the axis from (axisMode & 0xFF) - 0x58:
 *   0/0x20 -> rotation about X, 1/0x21 -> about Y, 2/0x22 -> about Z.
 * sinTerm/cosTerm are the precomputed sin/cos of the rotation angle.
 */
void func_80046188(GameRenderAxisMatrix *out, s32 sinTerm, s32 cosTerm, s32 axisMode) {
    s32 one;

    switch ((axisMode & 0xFF) - 0x58) {
    case 0:   /* X-axis rotation */
    case 32:
        one = 0x1000;
        out->m[0][0] = one;
        one = -sinTerm;
        out->m[0][1] = 0;
        out->m[0][2] = 0;
        out->m[1][0] = 0;
        out->m[1][1] = cosTerm;
        out->m[1][2] = one;
        out->m[2][0] = 0;
        out->m[2][1] = sinTerm;
        out->m[2][2] = cosTerm;
        break;

    case 1:   /* Y-axis rotation */
    case 33:
        one = 0x1000;
        out->m[1][1] = one;
        one = -sinTerm;
        out->m[0][0] = cosTerm;
        out->m[0][1] = 0;
        out->m[0][2] = sinTerm;
        out->m[1][0] = 0;
        out->m[1][2] = 0;
        out->m[2][0] = one;
        out->m[2][1] = 0;
        out->m[2][2] = cosTerm;
        break;

    case 2:   /* Z-axis rotation */
    case 34:
        out->m[0][1] = -sinTerm;
        out->m[0][0] = cosTerm;
        out->m[0][2] = 0;
        out->m[1][0] = sinTerm;
        out->m[1][1] = cosTerm;
        out->m[1][2] = 0;
        out->m[2][0] = 0;
        out->m[2][1] = 0;
        out->m[2][2] = 0x1000;
        break;
    }
}
