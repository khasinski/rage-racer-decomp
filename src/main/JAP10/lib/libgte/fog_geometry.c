#include "common.h"

void SetFogNear(s32 near, s32 projection) {
    SetDQA(-((near * 5) << 6) / projection);
    SetDQB(0x1400000);
}

s32 CordicRotate(s32 value) {
    s32 data[16];
    s32 *hi;
    register s32 *lo asm("$6");
    s32 i;
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
    lo[0] = value + magic;
    hi[0] = value - magic;

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

s32 SquareRoot12(s32 square) {
    s32 bits;
    s32 shift;
    s32 value;
    s32 ret;

    if (square == 0) {
        return 0;
    }

    bits = 8 - Lzc(square);
    if (bits >= 0) {
        shift = bits >> 1;
        value = square >> (shift * 2);
    } else {
        shift = (bits >> 1) + 1;
        value = square << -(((bits >> 1) + 1) * 2);
    }

    shift -= 6;
    if (shift < 0) {
        ret = CordicRotate(value) >> -shift;
    } else {
        ret = CordicRotate(value) << shift;
    }

    return ret;
}

/*
 * Original PSY-Q hardware initialiser.  The COP0/COP2 transfers are the
 * hardware interface, while the two symbols keep the JAP10 layout movable.
 */
void InitGeom(void) {
    asm volatile(
        ".set noreorder\n"
        ".set noat\n"
        "lui $1,%hi(D_800941B4)\n"
        "sw $31,%lo(D_800941B4)($1)\n"
        ".word 0x0C000000\n"
        ".reloc .-4,R_MIPS_26,func_80069804\n"
        "nop\n"
        "lui $31,%hi(D_800941B4)\n"
        "lw $31,%lo(D_800941B4)($31)\n"
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
