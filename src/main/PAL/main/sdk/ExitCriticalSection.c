#include <sys/types.h>

#include "common.h"
#include "game/random.h"

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
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/sdk/ExitCriticalSection", func_80063220);

u_long func_80063230[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090032,
    0,
};

u_long BiosFileSeek[4] asm("func_80063240") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090033,
    0,
};

u_long BiosFileRead[4] asm("func_80063250") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090034,
    0,
};

u_long BiosFileWrite[4] asm("func_80063260") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090035,
    0,
};

u_long BiosFileClose[4] asm("func_80063270") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090036,
    0,
};

u_long BiosFormatDevice[4] asm("func_80063280") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090041,
    0,
};

u_long BiosFirstFile[4] asm("func_80063290") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090042,
    0,
};

u_long BiosNextFile[4] asm("func_800632A0") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090043,
    0,
};

extern u32 g_RandomSeed asm("D_8009B9A8");

/* The game's own PRNG, not SDK code: it keeps common.h's typedefs to agree
 * with its declaration in game/random.h. */
s32 GameRandom15(void) {
    u32 value = g_RandomSeed;

    value *= 0x41C64E6D;
    value += 0x3039;
    g_RandomSeed = value;
    return (value >> 0x10) & 0x7FFF;
}
