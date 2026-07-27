#include "common.h"

/*
 * Names for this unit's hand-written GTE engine routines. Declarations only -
 * the emitted symbols stay func_XXXXXXXX, so the ROM is unaffected. What each
 * routine emits is worked out in docs/names.md section 17.
 */
void GameEmitPolyG4(void) asm("func_80029158");
void GameEmitPolyGT4(void) asm("func_80029230");
void GameSubmitModel2(void *ctx, s32 model) asm("func_80029340");
void GameEmitPolyF4Fog(void) asm("func_800293F0");
void GameEmitPolyFT4Fog(void) asm("func_80029458");
void GameEmitPolyG4Fog(void) asm("func_800294E4");
void GameEmitPolyGT4Fog(void) asm("func_800295BC");
void GameSubmitCourseModel(void *ctx, s32 model) asm("func_800296B4");

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80029158 = GameEmitPolyG4
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitPolyG4", func_80029158);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80029230 = GameEmitPolyGT4
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitPolyG4", func_80029230);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80029340 = GameSubmitModel2
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitPolyG4", func_80029340);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_800293F0 = GameEmitPolyF4Fog
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitPolyG4", func_800293F0);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80029458 = GameEmitPolyFT4Fog
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitPolyG4", func_80029458);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_800294E4 = GameEmitPolyG4Fog
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitPolyG4", func_800294E4);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_800295BC = GameEmitPolyGT4Fog
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitPolyG4", func_800295BC);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_800296B4
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    void func_800296B4(void *ctx, s32 arg);
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/gte/GameEmitPolyG4", func_800296B4);
