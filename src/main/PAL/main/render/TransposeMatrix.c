#include "common.h"
#include "psyq/gte.h"

u32 func_80069CBC[3] __attribute__((section(".text"))) = { 0, 0, 0 };

/*
 * 3x3 s16 matrix transpose: dst[i][j] = src[j][i].
 * Reads src words 0,3,6,1,4,7,2,5,8; writes dst 0..8; returns dst.
 *
 * Register pinning + two empty barriers (no ordinary-MIPS inline asm) are
 * needed to reproduce the retail allocation/scheduling exactly:
 *   - dstp -> $5 (a1) is the store base for every sh; ret -> $2 (v0) is only
 *     the return value; value0/1/2 -> $9/$10/$11 (t1/t2/t3) cycle the loads.
 *   - The first `__asm__ volatile("")` keeps `ret = dst` from floating above
 *     the first load so the resulting `move v0,a1` lands in that load's delay
 *     slot (offset 0x4), matching retail.
 *   - The second empty barrier (opaque identity on ret) hides $2==dst from the
 *     optimizer so it keeps addressing the stores through $5 instead of reusing
 *     $2 as the base pointer.
 */
Matrix *TransposeMatrix(Matrix *src, Matrix *dst) {
    s16 *srcp = (s16 *)src;
    register s16 *dstp asm("$5") = (s16 *)dst;
    register Matrix *ret asm("$2");
    register s32 value0 asm("$9");
    register s32 value1 asm("$10");
    register s32 value2 asm("$11");

    value0 = srcp[0];
    __asm__ volatile("");
    ret = dst;
    __asm__ volatile("" : "=r"(ret) : "0"(ret));
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

/*
 * func_80069D18 is RotMatrix: it builds a rotation MATRIX from an SVECTOR of
 * Euler angles using the packed cos/sin table at 0x80094FAC (cos in the high
 * halfword, sin in the low halfword; sin negated for negative angles).
 *
 * It is genuinely hand-written assembly in the original program and cannot be
 * produced from C with this toolchain: every fixed-point product uses a 32-bit
 * `multu` keeping only the low word (`multu; mflo; sra 12`). GCC 2.6.3/2.7.2
 * canonicalise every 32-bit-result multiply to signed `mult`, and only ever
 * emit `multu` for a true 64-bit widening multiply (which also reads `mfhi`).
 * No C expression yields `multu`+`mflo`-only, so a byte-exact C body is
 * impossible; it stays as raw asm to preserve the match. (See commit af632214,
 * which documents this exact function as one of the genuine hand-asm cases.)
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/render/TransposeMatrix", func_80069D18);
