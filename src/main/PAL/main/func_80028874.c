#include "common.h"

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80028874
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80028874);
