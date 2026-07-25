#include "common.h"

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80064588  (0x54D88, cc region 2.6.3, TU range 217 words)
 * Reason:   hand-written PSY-Q MDEC/DCT bitstream-decoder inner loop with a
 *           COP0 Status-register epilogue - not expressible as compiler C.
 * Evidence: (1) COP0 ops `mfc0 t1,$12` / `mtc0 t1,$12` read/modify/write the
 *           Status register - GCC emits no COP0 ops from ordinary C and these
 *           are not GTE/COP2 macros. (2) `b` encoded as `bgez $zero`
 *           (0x0401....) - the traditional/SN MIPS assembler encoding; the
 *           bundled GNU as emits `beq $zero,$zero`. (3) No stack frame / custom
 *           ABI: all working regs loaded from and stored to a global state
 *           struct at D_800648C8 - a resumable state machine, not the C ABI.
 *           (4) Signed `add` for pointer/index math where GCC emits `addu`.
 *           (5) hand `.set noreorder` delay-slot packing.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80064588);
