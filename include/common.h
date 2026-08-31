#ifndef RAGE_PC_COMMON_H
#define RAGE_PC_COMMON_H

#include "include_asm.h"
#include "asm_macros.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

/*
 * Word views. The retail code reads the same storage at two widths or two
 * signednesses all over the tree, and gcc 2.6.3 will not let a plain cast do
 * it: a union member reference is what makes it reload the word. Each of these
 * had been re-invented as a private typedef in a dozen headers and source
 * files under a dozen names; they are all the same device and live here.
 */
typedef union SplitWord {
    s32 value;
    struct {
        u16 low;
        u16 high;
    } half;
} SplitWord;

typedef union SignedWord {
    s32 value;
    u32 unsignedValue;
} SignedWord;

typedef union SignedHalf {
    u16 value;
    s16 signedValue;
} SignedHalf;

typedef union ShortPointer {
    s32 value;
    s16 *pointer;
} ShortPointer;


/*
 * Reads or writes a struct member without the compiler's "this lives inside a
 * struct" mark. That mark makes gcc 2.6.3 assume the access cannot alias a
 * plain global, which lets it move a neighbouring global store across the
 * access; the retail code keeps the two in source order. Going through an
 * integer round-trip defeats the fold back to a member reference, so the
 * access is just a load or store at a computed address, and the ordering is
 * restored without a memory clobber. Same address, same width, no barrier.
 */
/*
 * Register pins -- `register T x asm("$N")` and `asm("" : "=r"(x) : "0"(x))` --
 * and bare `asm("" ::: "memory")` barriers in this tree are load-bearing:
 * removing one changes .text. They are not annotated individually; assume any
 * of them matters and verify with `make check` before deleting.
 *
 * This has been measured exhaustively: every pure barrier and every pin was
 * removed in turn and the object compared against a baseline. Only a handful
 * turned out to be removable, and none hold each other - no function is free
 * when all its pins go at once, and no intra-function pair is removable
 * together.
 * Combinations across functions cannot matter, since register allocation is
 * per function.
 *
 * If you repeat that sweep, compare the *instruction stream*, not whole .o
 * files and not raw objdump text: -g -gcoff embeds line numbers and <LMnnn>
 * annotations, so any edit that changes the line count reports a false
 * difference. Two rounds of this tree's results were wrong for that reason.
 */

#define RAW(x) (*(__typeof__(x) *)((s32)&(x)))

#endif
