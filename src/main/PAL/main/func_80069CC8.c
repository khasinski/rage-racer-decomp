#include "common.h"
#include "psyq/gte.h"

Matrix *func_80069CC8(Matrix *src, Matrix *dst) {
    s16 *srcp = (s16 *)src;
    register s16 *dstp asm("$5") = (s16 *)dst;
    register Matrix *ret asm("$2");
    register s32 value0 asm("$9");
    register s32 value1 asm("$10");
    register s32 value2 asm("$11");

    value0 = srcp[0];
    asm("addu $2,$5,$0");
    dstp[0] = value0;
    value1 = srcp[3];
    value0 = srcp[6];
    dstp[1] = value1;
    value2 = srcp[1];
    dstp[2] = value0;
    value1 = srcp[4];
    dstp[3] = value2;
    value0 = srcp[7];
    dstp[4] = value1;
    value2 = srcp[2];
    dstp[5] = value0;
    value1 = srcp[5];
    dstp[6] = value2;
    value0 = srcp[8];
    dstp[7] = value1;
    dstp[8] = value0;
    return ret;
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80069CC8", func_80069D18);
