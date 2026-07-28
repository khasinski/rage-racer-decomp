#include "common.h"

/*
 * Installs the GTE-aware exception handler: takes the BIOS exception table
 * pointer from B0(0x56), then copies the handler body that lives between
 * func_8006A010 and func_8006A048 into it.
 *
 * Hand-written assembly. It was previously carried as a u_long array in
 * .text, which meant the two block bounds and the saved-$ra slot were baked
 * in as literals and did not relocate; see docs/names.md on shiftability.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/sdk/Gte_PatchExceptionHandler", func_80069FA8);
