#include "common.h"

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80063220
 * Address:  0x80063220 (PAL/main, retail range [0x53A20, 0x53A30))
 * Reason:   PSY-Q kernel syscall trampoline (ExitCriticalSection). Loads the
 *           syscall selector 2 into $a0 and executes a `syscall` instruction,
 *           then returns. The body is the raw BIOS-call sequence:
 *               addiu $a0, $zero, 2   (0x24040002)
 *               syscall               (0x0000000C)
 *               jr    $ra             (0x03E00008)
 *               nop
 * Evidence:
 *   - Contains a `syscall` instruction. The C language has no construct that
 *     emits `syscall`; it is a kernel-entry hardware operation, not compiler
 *     output.
 *   - No stack frame is set up or torn down, and $a0 is loaded with a fixed
 *     selector for the BIOS exception handler: a custom kernel-call ABI, not
 *     the ordinary C calling convention.
 *   - This is a PSY-Q library leaf (ExitCriticalSection) shipped as assembly.
 * Why C+PSYQ macros are insufficient: the routine is nothing but a `syscall`
 *   BIOS entry with a hardcoded selector; there is no surrounding C algorithm
 *   to express, and no PSYQ GTE/COP2 macro applies.
 * Current representation: generated assembly stub (INCLUDE_ASM).
 * Revisit condition: none expected; this is verbatim SDK/BIOS syscall glue.
 */
INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80063220);

u32 func_80063230[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090032,
    0,
};
