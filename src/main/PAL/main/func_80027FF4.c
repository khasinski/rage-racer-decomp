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
