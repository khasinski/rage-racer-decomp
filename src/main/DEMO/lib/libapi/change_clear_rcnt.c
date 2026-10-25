#include "common.h"

/* PsyQ kernel call trampoline; see src/main/PAL/lib/libapi/bios_call_stubs.c
 * for the raw jump-through-a-register protocol rationale. Not itself part of
 * retail's bios_call_stubs.s (ChangeClearRCnt is auto-matched to a different
 * position there), so this is its own tiny handwritten-asm unit here. */
HANDWRITTEN_ASM("src/main/DEMO/lib/libapi", change_clear_rcnt);
