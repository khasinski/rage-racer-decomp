#include "common.h"

/*
 * Names for this unit's hand-written GTE engine routines. Declarations only -
 * the emitted symbols stay func_XXXXXXXX, so the ROM is unaffected. What each
 * routine emits is worked out in docs/names.md section 17.
 */
void GameEmitCoursePolyF4(void) asm("func_8002970C");
void GameEmitCoursePolyFT4(void) asm("func_80029788");
void GameEmitCourseSubdividedFT4(void) asm("func_80029830");
void GameEmitCourseSubdividedFT4Fog(void) asm("func_800298B0");

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_8002970C = GameEmitCoursePolyF4
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitCoursePolyF4", func_8002970C);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80029788 = GameEmitCoursePolyFT4
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitCoursePolyF4", func_80029788);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80029830 = GameEmitCourseSubdividedFT4
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitCoursePolyF4", func_80029830);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_800298B0 = GameEmitCourseSubdividedFT4Fog
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitCoursePolyF4", func_800298B0);
