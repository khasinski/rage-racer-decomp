#include "common.h"

/*
 * PAL executable entry and PsyQ BIOS jump table.
 *
 * D_800630B4 is the address the PS-EXE header names as the initial pc.  It
 * clears the region from the start of zeroed data up to main_BSS_END, sets
 * $gp, $sp and $fp, hands the memory left over to InitHeap and calls main.
 * The labels that follow it are callable BIOS A0/B0 stubs whose register
 * protocol cannot be expressed as normal C, plus the four-word stack-top
 * table at D_80063160 that the entry code indexes.
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
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/boot/_start", D_800630B4);
