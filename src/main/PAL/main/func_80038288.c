#include "common.h"

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80038288
 * Address:  0x80038288 (PAL/main)
 * Reason:   Tunnel/proximity audio-pitch helper. The surrounding arithmetic is
 *           ordinary C, but the routine ends with a multiply whose product is
 *           never consumed before the function returns.
 * Evidence:
 *   - Dead `mult v0, v1` at the tail (v1 = -0x40): the retail epilogue
 *     (lw ra / lw s0 / addiu sp / jr ra) follows immediately with no mflo/mfhi
 *     and no fall-through consumer. The HI/LO product is discarded.
 *   - No -O2 C compiler emits a pure dead multiply: GCC dead-code-eliminates
 *     any multiply whose result is unused and expressible in C. The retail
 *     bytes therefore cannot be produced from byte-exact plain C - the dead
 *     multiply only exists because it was written by hand.
 * Why C+PSYQ macros are insufficient: a dead multiply is not expressible in C
 *   without an inline-asm `mult` crutch; keeping it as C would require the very
 *   inline assembly this policy forbids for a plain-C match.
 * Current representation: register-pinned C with a single inline-asm `mult`
 *   reproducing the deliberate dead multiply. Byte-exact (tucheck DIFFS=0).
 * Revisit condition: evidence that the product is consumed (e.g. a caller that
 *   reads HI/LO), which would make it ordinary C.
 */

extern s32 D_801E40D8;

s32 func_8001A6AC(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);

s32 func_80038288(s32 arg0) {
    register s32 trackLength asm("a1");
    register s32 value asm("v1");
    register s32 temp asm("v0");
    register s32 angle asm("s0");
    register s32 remainder asm("a0");
    register s32 scale asm("v1");

    trackLength = D_801E40D8;
    temp = 0xB875;
    value = trackLength + temp;
    value -= arg0;
    remainder = value % trackLength;
    value = remainder;

    if (value < 0x1A97) {
        temp = value / 100;
        value = 0x80 - temp;
    } else {
        value = trackLength - remainder;
        if (value < 0x801) {
            temp = value / 10;
            value = 0x80 - temp;
        } else {
            value = 0;
        }
    }

    if (value != 0) {
        temp = func_8001A6AC(0x29DD - *(s32 *)0x1F800008, 0x6EF3 - *(s32 *)0x1F800010);
        value = 0xC00 - temp;
        temp = *(s32 *)0x1F80001C;
        value &= 0xFFF;
        temp -= value;
        angle = temp & 0xFFF;
        temp = func_80068568(angle);
        temp = func_80068568(angle);
        scale = -0x40;
        __asm__ volatile("mult %0,%1" : : "r"(temp), "r"(scale));
        asm(".globl func_80038368\nfunc_80038368 = func_80038288 + 0xE0");
    }
}
