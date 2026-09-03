#include "common.h"
#include "psyq/gte.h"


/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80069D18 = RotMatrix (PSY-Q libgte; Sony's Run-Time Library
 *           Reference, 8-140). Builds a rotation MATRIX from an SVECTOR of
 *           Euler angles using the packed cos/sin table at g_RCosSinTable (cos in
 *           the high halfword, sin in the low halfword; sin negated for
 *           negative angles).
 * Reason:   hand-written PSY-Q libgte SDK assembly, not compiler C.
 * Evidence: every fixed-point product is a NARROW unsigned multiply keeping
 *           only the low word - 14x `multu`, 15x `mflo`, and ZERO `mfhi` in
 *           the whole body. GCC 2.6.3 and 2.7.2 (this repo's cc1) canonicalise
 *           every truncated 32-bit multiply to signed `mult`, and only emit
 *           `multu` for a true 64-bit widening multiply, which always reads
 *           `mfhi`. `multu` with no `mfhi` is therefore unreachable from C,
 *           and no matched function in this project emits it. The body also
 *           loads the packed table entry as one `lw` and splits it with
 *           `sll 16; sra 16` / `sra 16` where cc1 folds such halfword
 *           extraction into `lh`, and joins its if/else arms with a bare `j`
 *           whose delay slot carries the shared `sra $t1,$t9,16`.
 *           Exact same idiom and register file (t0-t9 only) as the documented
 *           siblings func_80069110 (ScaleMatrixL) and func_80069458.
 * Revisit:  only if the exact cc1 variant that emits narrow multu is obtained
 *           AND verified not to regress already-matched functions.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libgte/rot_matrix_asm", RotMatrix);
