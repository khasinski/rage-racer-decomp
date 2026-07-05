#include "common.h"

s32 Lzc(s32 arg0) asm("func_80069C7C");

s32 func_80068738(s32 arg0) {
    s32 data[16];
    register s32 *hi asm("$8");
    register s32 *lo asm("$6");
    register s32 i asm("$7");
    register s32 offset asm("$5");
    register s32 *dst asm("$4");
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
                asm volatile("addu %0,%1,%2" : "=r"(dst) : "r"(hi), "r"(offset));
                lo[1] = loValue - shifted;
                *dst = lo[8] - (lo[0] >> i);
            } else {
                asm volatile(
                    "srav $2,%2,%3\n"
                    "lw $3,0(%4)\n"
                    "addu %0,%5,%1\n"
                    "addu $2,$2,$3\n"
                    "sw $2,4(%4)"
                    : "=r"(dst)
                    : "r"(offset), "r"(temp), "r"(i), "r"(lo), "r"(hi)
                    : "memory");
                *dst = (lo[0] >> i) + lo[8];
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

s32 func_8006888C(s32 arg0) {
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
