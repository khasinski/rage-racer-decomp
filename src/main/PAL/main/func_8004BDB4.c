#include "common.h"

extern u32 D_801E6F2C[];

void func_8005D6EC(s32 arg0);

void func_8004BDB4(void) {
    register s32 i asm("$8");
    register s32 j asm("$9");
    register s32 k asm("$7");
    register s32 limit asm("$12");
    register u32 *base asm("$13");
    register u32 *srcStart asm("$10");
    register u32 *src asm("$6");
    register u32 *stackBase asm("$11");
    register u32 *dst asm("$4");
    register s32 shift asm("$5");
    register u32 value1 asm("$3");
    register u32 value2 asm("$2");
    u32 saved[512];

    func_8005D6EC(8);

    base = D_801E6F2C;
    i = 0;
    limit = 7;
    do {
        j = 0;
        value2 = limit - i;
        value2 <<= 2;
        asm("addu %0,%1,%2" : "=r"(srcStart) : "r"(value2), "r"(base));
        do {
            k = 0;
            src = srcStart;
            do {
                dst = (u32 *)((((i * 8 + k) * 8 + j) << 2) + (s32)(stackBase = saved));
                shift = (limit - k) << 2;
                *dst = 0;
                value1 = (src[0x38] >> shift) & 0xF;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x30] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x28] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x20] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x18] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x10] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x08] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x00] >> shift) & 0xF;
                k++;
                value2 |= value1;
                *dst = value2;
            } while (k < 8);
            j++;
            srcStart += 0x40;
        } while (j < 8);
        i++;
    } while (i < 8);

    i = 0;
    dst = base;
    value1 = (u32)stackBase;
    do {
        value2 = *(u32 *)value1;
        value1 += 4;
        i++;
        *dst = value2;
        dst++;
    } while (i < 512);
}

asm(".globl func_8004BDEC\nfunc_8004BDEC = func_8004BDB4 + 0x38");
