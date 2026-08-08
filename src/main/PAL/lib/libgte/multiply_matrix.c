#include "common.h"

/*
 * PSY-Q 3.5 libgte object mtx_03.o (LIBGTE.A): MulMatrix (func_80069458).
 * Matrix x matrix multiply that stores the product back into m0.
 *
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 * Hand-written PSY-Q libgte routine (.set noreorder style): the `lui $at;
 * and rN,rN,$at` 0xffff0000 masks via the reserved assembler temp $at (cc1
 * never emits $at) and a deliberately unfilled `jr ra; nop` delay slot are an
 * irreducible handwritten-asm signature GCC 2.6.3 will not reproduce from C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libgte/multiply_matrix", MulMatrix);
