#include "common.h"

/*
 * Installs the GTE-aware exception handler: takes the BIOS exception table
 * pointer from B0(0x56), then copies the handler body that lives between
 * func_8006A010 and func_8006A048 into it.
 *
 * Hand-written assembly. It was previously carried as a u_long array in
 * .text, which meant the two block bounds and the saved-$ra slot were baked
 * in as literals and did not relocate.
 */
/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80069FA8 = Gte_PatchExceptionHandler (PSY-Q libgte).
 * Reason:   hand-written assembly, and deliberately kept that way. It reads the
 *           BIOS exception table through B0(0x56) and copies a handler body into
 *           it, so it manipulates addresses of its own code.
 * Evidence: it too was carried as a u_long array, which baked the two block
 *           bounds and the saved-$ra slot in as literals that did not relocate.
 *           As real assembly those became relocation records and the block
 *           follows the image when code size changes. Decompiling it would
 *           undo that.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libgte/exception_patch", func_80069FA8);
