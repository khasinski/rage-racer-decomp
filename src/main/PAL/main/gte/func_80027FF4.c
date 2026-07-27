#include "common.h"

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80027FF4  (cc=2.7.2 region, part of the GTE dispatch engine
 *           adjacent to func_80028120..func_8002A2CC)
 * Reason:   hand-written GTE dispatch loop, outside the O32 C ABI.
 * Evidence: NO stack frame - the function never touches $sp. The prologue saves
 *           the return address and callee-saved regs into fields of the
 *           caller-supplied context struct at $a0 instead of the stack:
 *               sw ra,0x8c(a0)  sw s0,0x94(a0)  sw s1,0x98(a0)
 *           and reloads them from the struct before each `jr ra`. $v0 is also
 *           preserved across both `jal`s via the struct (sw v0,0x80(a0) /
 *           lw v0,0x80(a0)) where GCC would use an $sp slot. The $a0 struct
 *           doubles as live data and register-save area - a hand-coded design.
 *           GCC 2.x never uses an incoming argument pointer as its save area.
 * C view:   not expressible as compiler C. Same class as the func_8002xxxx
 *           GTE engine and the sibling func_80028E9C.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80027FF4);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80028120
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80028120);

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

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80028C54
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80028C54);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80028D84
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80028D84);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80028DEC
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80028DEC);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 * Hand-written GTE face loop. This is outside the o32 C ABI: sixteen slt
 * instructions write to the assembler-reserved $at register, and the
 * dispatch path jumps through $t2 while passing live values in $t6-$t9.
 * Keep the original instructions until this custom-ABI group is rewritten.
 */
asm(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80028E9C\n"
    "func_80028E9C:\n"
    ".word 0x30AFFFFF\n"
    ".word 0x000F78C0\n"
    ".word 0x3C0E8007\n"
    ".word 0x35CEDA14\n"
    ".word 0x01CFC821\n"
    ".word 0x8F2A0000\n"
    ".word 0x8F2B0004\n"
    ".word 0x00052C02\n"
    ".word 0x8C860000\n"
    ".word 0x8C880080\n"
    ".word 0x8C890054\n"
    ".word 0x8C8D0004\n"
    ".word 0x8C8C0064\n"
    ".word 0x25AD0200\n"
    ".word 0x8D0E0000\n"
    ".word 0x8D180004\n"
    ".word 0x000E7C02\n"
    ".word 0x31CEFFFF\n"
    ".word 0x0018CC02\n"
    ".word 0x3318FFFF\n"
    ".word 0x000E70C0\n"
    ".word 0x000F78C0\n"
    ".word 0x0018C0C0\n"
    ".word 0x01C97021\n"
    ".word 0x01E97821\n"
    ".word 0x0309C021\n"
    ".word 0xC9C00000\n"  /* lwc2 */
    ".word 0xC9C10004\n"  /* lwc2 */
    ".word 0xC9E20000\n"  /* lwc2 */
    ".word 0xC9E30004\n"  /* lwc2 */
    ".word 0xCB040000\n"  /* lwc2 */
    ".word 0xCB050004\n"  /* lwc2 */
    ".word 0x0019C8C0\n"
    ".word 0x4A280030\n"  /* gte_rtpt (RTPT) */
    ".word 0x8C830068\n"
    ".word 0x0329C821\n"
    ".word 0x4B400006\n"  /* gte_nclip (NCLIP) */
    ".word 0x14600005\n"
    ".word 0x4802C000\n"  /* mfc2 */
    ".word 0x00000000\n"
    ".word 0x1C400004\n"
    ".word 0x00000000\n"
    ".word 0x10000040\n"
    ".word 0x00000000\n"
    ".word 0x0441003E\n"
    ".word 0xE88C0100\n"  /* swc2 */
    ".word 0xE88D0104\n"  /* swc2 */
    ".word 0xE88E0108\n"  /* swc2 */
    ".word 0xCB200000\n"  /* lwc2 */
    ".word 0xCB210004\n"  /* lwc2 */
    ".word 0x8482007C\n"
    ".word 0x4A180001\n"  /* gte_rtps (RTPS) */
    ".word 0x84830078\n"
    ".word 0x848E0100\n"
    ".word 0x848F0104\n"
    ".word 0xE88E010C\n"  /* swc2 */
    ".word 0x84980108\n"
    ".word 0x4B68002E\n"  /* gte_avsz4 (AVSZ4) */
    ".word 0x8499010C\n"
    ".word 0x48073800\n"  /* mfc2 */
    ".word 0x004E082A\n"  /* slt $at, $v0, $t6 */
    ".word 0x10200006\n"
    ".word 0x004F082A\n"  /* slt $at, $v0, $t7 */
    ".word 0x10200004\n"
    ".word 0x0058082A\n"  /* slt $at, $v0, $t8 */
    ".word 0x10200002\n"
    ".word 0x0059082A\n"  /* slt $at, $v0, $t9 */
    ".word 0x14200027\n"
    ".word 0x01C3082A\n"  /* slt $at, $t6, $v1 */
    ".word 0x10200006\n"
    ".word 0x01E3082A\n"  /* slt $at, $t7, $v1 */
    ".word 0x10200004\n"
    ".word 0x0303082A\n"  /* slt $at, $t8, $v1 */
    ".word 0x10200002\n"
    ".word 0x0323082A\n"  /* slt $at, $t9, $v1 */
    ".word 0x1420001F\n"
    ".word 0x8482007E\n"
    ".word 0x8483007A\n"
    ".word 0x848E0102\n"
    ".word 0x848F0106\n"
    ".word 0x8498010A\n"
    ".word 0x8499010E\n"
    ".word 0x004E082A\n"  /* second vertex set: same $at comparisons */
    ".word 0x10200006\n"
    ".word 0x004F082A\n"
    ".word 0x10200004\n"
    ".word 0x0058082A\n"
    ".word 0x10200002\n"
    ".word 0x0059082A\n"
    ".word 0x14200011\n"
    ".word 0x01C3082A\n"
    ".word 0x10200006\n"
    ".word 0x01E3082A\n"
    ".word 0x10200004\n"
    ".word 0x0303082A\n"
    ".word 0x10200002\n"
    ".word 0x0323082A\n"
    ".word 0x14200009\n"
    ".word 0x01873807\n"
    ".word 0x24E2FFFF\n"
    ".word 0x2C4201BF\n"
    ".word 0x10400005\n"
    ".word 0x8C8E0100\n"
    ".word 0x8C8F0104\n"
    ".word 0x8C980108\n"
    ".word 0x01400008\n"  /* jr $t2; $t6-$t9 are live-out arguments */
    ".word 0x8C99010C\n"
    ".word 0x24A5FFFF\n"
    ".word 0x14A0FFA1\n"
    ".word 0x010B4021\n"
    ".word 0xAC880080\n"
    ".word 0xAC860000\n"
    ".word 0x03E00008\n"
    ".word 0x00000000\n"
);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_80029064
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80029064);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Part of the scratchpad (0x1F800000) GTE geometry/command engine, dispatched
 * through the jump tables jtbl_8007D9F4 / jtbl_8007DA14. Not compiler-generated
 * C; must stay as assembly.
 *
 * Symbol:    func_800290C8
 * Reason:    hand-written GTE engine routine.
 * Evidence:  custom calling convention (arguments/state passed in t0/t6-t9/a2,
 *            not the O32 argument registers); return address stored in a
 *            scratchpad field instead of the stack (no stack frame); registers
 *            preserved across internal calls; shared code reached through
 *            multiple mid-routine entry points; heavy COP2/GTE usage.
 * C view:    internal engine routine (custom ABI); not called directly from C.
 * Revisit:   only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_800290C8);
