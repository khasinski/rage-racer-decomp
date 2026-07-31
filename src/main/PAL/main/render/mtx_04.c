#include "common.h"

/*
 * PSY-Q 3.5 libgte object mtx_04.o (LIBGTE.A): MulMatrix2 (func_80069568).
 * Matrix x matrix multiply that stores the product back into m1.
 *
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 * Hand-written PSY-Q libgte routine (.set noreorder style); sibling of mtx_03
 * MulMatrix. The `lui $1,0xffff; and rN,rN,$1` masks (reserved assembler temp
 * $at, never allocated by cc1) and a t0-t8-only register file are an
 * irreducible handwritten-asm signature not reachable from compiler C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/render/mtx_04", func_80069568);
