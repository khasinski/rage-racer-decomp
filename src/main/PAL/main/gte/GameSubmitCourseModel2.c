#include "common.h"

/* Name for this unit's hand-written GTE engine routine. Declaration only -
 * the emitted symbol stays func_80029E50. */
void GameSubmitCourseModel2(void *ctx, s32 model) asm("func_80029E50");

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80029E50 = GameSubmitCourseModel2
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    void func_80029E50(void *ctx, s32 arg);
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameSubmitCourseModel2", func_80029E50);
