#include "common.h"

/*
 * PERMUTER-CANDIDATE (DIFFS=13): complete real C at
 * scratch/permuter-seeds/func_80057748.wip.c. Sole blocker = GCC 2.6.3 cse
 * unifies the literal 2 (ternary + switch case) into one callee-saved pseudo
 * vs retail's two rematerialized `li v0,2`. Confirmed register-coloring only
 * (compiled 572 vs retail 573 = the deduped li). Hand levers exhausted
 * (opaque-identity barrier both sides -> 570/576, worse); needs reloc-aware
 * permuter. Byte-exact INCLUDE_ASM until then.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80057748", func_80057748);
