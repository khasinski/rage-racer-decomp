#include "common.h"

/*
 * Applies the 3x3 fixed-point matrix `mtx` to the vector (x,y,z), writing each
 * component to *outX/*outY/*outZ (result >> 12). Each row has an identity
 * fast-path: if the row is (1.0, 0, 0) etc. the matching input component is
 * copied straight through without the multiply-accumulate.
 */
void MatrixApplyVectorComponents(s16 *mtx, s32 x, s32 y, s32 z, s32 *outX, s32 *outY, s32 *outZ) asm("func_800681F0");
void MatrixApplyVectorComponents(s16 *mtx, s32 x, s32 y, s32 z, s32 *outX, s32 *outY, s32 *outZ) {
    s16 *m = mtx;

    if (*(s32 *)&m[0] == 0x1000 && m[2] == 0) {
        *outX = x;
    } else {
        s32 result = m[0] * x;

        result += m[1] * y;
        result += m[2] * z;
        *outX = result >> 12;
    }

    if (m[3] == 0 && *(s32 *)&m[4] == 0x1000) {
        *outY = y;
    } else {
        s32 result = m[3] * x;

        result += m[4] * y;
        result += m[5] * z;
        *outY = result >> 12;
    }

    if (*(s32 *)&m[6] == 0 && m[8] == 0x1000) {
        *outZ = z;
    } else {
        s32 result = m[6] * x;

        result += m[7] * y;
        result += m[8] * z;
        *outZ = result >> 12;
    }
}
