#include <sys/types.h>
#include <stdio.h>

#include "common.h"
#include "psyq/cd.h"
#include "psyq/press_internal.h"


/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   g_MdecVlcBufferSize  (0x54D54, 13 words)
 * Reason:   a mutable data word embedded in .text immediately followed by the
 *           function code that reads/writes it via its own absolute address
 *           (t0 = 0x80064554; lw/sw 0(t0)). The leading word (init 0x00FFFFFF)
 *           lives inside the code section, before the routine.
 * Evidence: standard C cannot place a mutable global inline in .text ahead of a
 *           function (globals go to .data/.sdata/.bss, const to .rodata - never
 *           interleaved with code). The self-referential absolute load/store of
 *           its own symbol address confirms a hand-assembled data+code block.
 * C view:   not expressible as compiler C; effectively `static long state =
 *           0x00FFFFFF;` plus a small accessor, but the .text layout is not
 *           reproducible by GCC.
 * Revisit:  only if a linker/section arrangement can reproduce the exact layout.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libpress/mdec_unpack_status", g_MdecVlcBufferSize);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   MdecUnpackStatus  (0x54D88, cc region 2.6.3, TU range 217 words)
 * Reason:   hand-written PSY-Q MDEC/DCT bitstream-decoder inner loop with a
 *           COP0 Status-register epilogue - not expressible as compiler C.
 * Evidence: (1) COP0 ops `mfc0 t1,$12` / `mtc0 t1,$12` read/modify/write the
 *           Status register - GCC emits no COP0 ops from ordinary C and these
 *           are not GTE/COP2 macros. (2) `b` encoded as `bgez $zero`
 *           (0x0401....) - the traditional/SN MIPS assembler encoding; the
 *           bundled GNU as emits `beq $zero,$zero`. (3) No stack frame / custom
 *           ABI: all working regs loaded from and stored to a global state
 *           struct at g_MdecVlcResumeState - a resumable state machine, not the C ABI.
 *           (4) Signed `add` for pointer/index math where GCC emits `addu`.
 *           (5) hand `.set noreorder` delay-slot packing.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libpress/mdec_unpack_status", MdecUnpackStatus);
