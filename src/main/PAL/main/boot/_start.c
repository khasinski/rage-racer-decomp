#include "common.h"

/*
 * PAL executable entry and PsyQ BIOS jump table.
 *
 * D_800630B4 is the address the PS-EXE header names as the initial pc.  It
 * clears the region from the start of zeroed data up to main_BSS_END, sets
 * $gp, $sp and $fp, hands the memory left over to InitHeap and calls main.
 * The labels that follow it are callable BIOS A0/B0 stubs whose register
 * protocol cannot be expressed as normal C, plus the four-word stack-top
 * table at g_BootStackTopTable that the entry code indexes.
 *
 * This block used to be a `u32[]` placed in .text with __attribute__.  That
 * spelling froze the image: an array of literal words carries no relocations,
 * so the addresses encoded in it - the end of bss, the two data words, its own
 * stack table, the calls to InitHeap and to main - kept naming the retail
 * layout no matter where the linker had put any of them.  Growing .text by a
 * single word left the entry stub jumping into the middle of some other
 * function.  As real assembly the assembler emits %hi/%lo and jump
 * relocations and the linker resolves them against wherever the symbols
 * actually landed.
 */
/*
 * HANDWRITTEN_ASM - excluded from progress; see README.md.
 *
 * Symbol:   D_800630B4 = the PAL entry point named by the PS-EXE header.
 * Reason:   hand-written assembly, and deliberately kept that way. It runs
 *           before $gp, $sp and $fp exist, so it cannot be expressed as C, and
 *           the BIOS A0/B0 stubs after it have a protocol C cannot
 *           state either.
 * Evidence: the same block used to be a u32[] in .text. As literal words it
 *           carried no relocations, so a build whose code changed size still
 *           jumped to the old addresses and hung at 0x8006317x. Rewriting it as
 *           real assembly gave it 28 relocation records and made the image
 *           shiftable, which was proved by sideloading a +0x40 build against the
 *           retail disc: every sampled pc was the baseline's plus exactly 0x40.
 *           Decompiling it would undo that.
 */
INCLUDE_ASM_TU("src/main/PAL/main/boot", _start);
