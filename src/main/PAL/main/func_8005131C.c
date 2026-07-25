#include "common.h"

/*
 * PERMUTER-CANDIDATE: complete real-C decompilation exists (see
 * scratch/decomp-work/func_8005131C/, ~140 diffs) - not blocked, not
 * handwritten. Residual is a coupled register-allocation/scheduling artifact
 * (the div/diff pseudo wants $s3 but that forces a 5th callee reg + a prologue
 * word, cascading positional diffs). Kept as byte-exact INCLUDE_ASM until a
 * permuter run converges.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_8005131C);
