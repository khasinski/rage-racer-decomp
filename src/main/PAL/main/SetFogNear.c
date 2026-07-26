#include "common.h"

void func_80069A00(s32 arg0);
void func_80069A0C(s32 arg0);

void SetFogNear(s32 arg0, s32 arg1) asm("func_800686D4");
void SetFogNear(s32 arg0, s32 arg1) {
    func_80069A00(-((arg0 * 5) << 6) / arg1);
    func_80069A0C(0x1400000);
}

s32 Lzc(s32 arg0) asm("func_80069C7C");

s32 func_80068738(s32 arg0) {
    s32 data[16];
    register s32 *hi asm("$8");
    register s32 *lo asm("$6");
    register s32 i asm("$7");
    s32 offset;
    s32 temp;
    s32 nextLo;
    s32 nextHi;
    s32 loValue;
    s32 shifted;
    s32 oldShift;
    s32 magic;

    magic = 0x5D50AD;
    i = 1;
    hi = &data[9];
    lo = &data[1];
    lo[0] = arg0 + magic;
    hi[0] = arg0 - magic;

    do {
        if (i != 4) {
            temp = lo[8];
            offset = i << 2;
            if (temp >= 0) {
                shifted = temp >> i;
                loValue = lo[0];
                lo[1] = loValue - shifted;
                *(s32 *)((u8 *)hi + offset) = lo[8] - (lo[0] >> i);
            } else {
                lo[1] = (temp >> i) + lo[0];
                *(s32 *)((u8 *)hi + offset) = (lo[0] >> i) + lo[8];
            }
        } else {
            temp = data[12];
            if (temp >= 0) {
                nextLo = data[4] - (temp >> 4);
                oldShift = data[4] >> 4;
                nextHi = temp - oldShift;
                data[4] = nextLo;
                data[12] = nextHi;
                if (nextHi >= 0) {
                    data[5] = nextLo - (nextHi >> 4);
                    data[13] = nextHi - (nextLo >> 4);
                } else {
                    data[5] = (nextHi >> 4) + nextLo;
                    data[13] = (nextLo >> 4) + nextHi;
                }
            } else {
                nextLo = (temp >> 4) + data[4];
                oldShift = data[4] >> 4;
                nextHi = oldShift + temp;
                data[4] = nextLo;
                data[12] = nextHi;
                if (nextHi >= 0) {
                    data[5] = nextLo - (nextHi >> 4);
                    data[13] = nextHi - (nextLo >> 4);
                } else {
                    data[5] = (nextHi >> 4) + nextLo;
                    data[13] = (nextLo >> 4) + nextHi;
                }
            }
        }

        i++;
        lo++;
    } while (i < 7);

    return data[7];
}

s32 SquareRoot12(s32 arg0) asm("func_8006888C");
s32 SquareRoot12(s32 arg0) {
    s32 bits;
    s32 shift;
    s32 value;
    s32 ret;

    if (arg0 == 0) {
        return 0;
    }

    bits = 8 - Lzc(arg0);
    if (bits >= 0) {
        shift = bits >> 1;
        value = arg0 >> (shift * 2);
    } else {
        shift = (bits >> 1) + 1;
        value = arg0 << -(((bits >> 1) + 1) * 2);
    }

    shift -= 6;
    if (shift < 0) {
        ret = func_80068738(value) >> -shift;
        goto done;
    }

    ret = func_80068738(value) << shift;

done:
    return ret;
}

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */

void func_80068928(void) {
    asm volatile(
        ".set noreorder\n"
        ".set noat\n"
        "lui $1,0x8009\n"
        "sw $31,0x4B0C($1)\n"
        ".word 0x0C01A7EA\n"
        "nop\n"
        "lui $31,0x8009\n"
        "lw $31,0x4B0C($31)\n"
        "nop\n"
        "mfc0 $2,$12\n"
        "lui $3,0x4000\n"
        "or $2,$2,$3\n"
        "mtc0 $2,$12\n"
        "nop\n"
        "addiu $8,$0,0x155\n"
        "ctc2 $8,$29\n"
        "nop\n"
        "addiu $8,$0,0x100\n"
        "ctc2 $8,$30\n"
        "nop\n"
        "addiu $8,$0,0x3E8\n"
        "ctc2 $8,$26\n"
        "nop\n"
        "addiu $8,$0,0xEF9E\n"
        "ctc2 $8,$27\n"
        "nop\n"
        "lui $8,0x140\n"
        "ctc2 $8,$28\n"
        "nop\n"
        "ctc2 $0,$24\n"
        "ctc2 $0,$25\n"
        "nop");
}

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_800689A8
 * Address:  0x800689A8 (PAL/main, retail range [0x591A8, 0x5922C))
 * Reason:   GTE-LZC fixed-point square-root helper. Reads leading-sign-bit
 *           count from LZCR ($31), normalises the operand by an even shift,
 *           looks up a mantissa table at 0x80094B1C, then rescales by half the
 *           exponent. The algorithm is expressible in C, but the retail bytes
 *           are not compiler output.
 * Evidence:
 *   - `li at,32; beq v0,at` holds $1 (assembler temp) as a live comparison
 *     operand across the branch. GCC reserves $1 and never allocates it to a
 *     value -> invalid register for compiler-generated C.
 *   - Trapping `addi`/`sub` (4x: 0x2c,0x34,0x4c,0x54). The rebuilt cc1-psx
 *     (both 2.6.3 and 2.7.2) only ever emits non-trapping `addiu`/`subu`;
 *     verified against every committed matched function.
 *   - Dead `andi t0,v0,1` (0x1c): result never consumed; no -O2 compiler
 *     emits a pure dead computation here.
 *   - Unconditional `b` (beq $0,$0) at 0x44 where the compiler emits `j`.
 *   - Unfilled branch delay slots (nop at 0x18 after beq, 0x3c after bltz).
 *   - Two independent `jr ra` epilogues (0x74 and 0x7c) rather than a single
 *     shared/cross-jumped return.
 * Why C+PSYQ macros are insufficient: the trapping arithmetic and the live use
 *   of $1 cannot be produced by the available compilers, so no C source can be
 *   byte-exact. A register-pinned C rewrite reaches the correct registers and
 *   algorithm but still floors at ~22 instruction diffs from these tool-level
 *   differences.
 * Current representation: generated assembly stub (INCLUDE_ASM).
 * Revisit condition: an original PSY-Q ccpsx (trapping-arith) cc1 becomes
 *   available to the build, or the routine is confirmed as a shippable
 *   library .s.
 */
INCLUDE_ASM("asm/nonmatchings/PAL/main", func_800689A8);
