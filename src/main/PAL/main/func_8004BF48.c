#include "common.h"

extern u32 D_801E6F2C[];

void func_8005D6EC(s32 arg0);

void func_8004BF48(void) {
    register s32 i asm("$8");
    register s32 j asm("$9");
    register s32 k asm("$7");
    register u32 *rowBase asm("$12");
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

    i = 0;
    base = D_801E6F2C;
    rowBase = base;
    do {
        j = 0;
        srcStart = rowBase;
        do {
            k = 0;
            src = srcStart;
            do {
                dst = (u32 *)((i * 8 + k) * 8);
                asm("addiu %0,%1,7" : "=r"(dst) : "r"(dst));
                asm("subu %0,%1,%2" : "=r"(dst) : "r"(dst), "r"(j));
                dst = (u32 *)(((s32)dst << 2) + (s32)(stackBase = saved));
                *dst = 0;
                value1 = src[0x00];
                shift = k << 2;
                value1 = (value1 >> shift) & 0xF;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x08] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x10] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x18] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x20] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x28] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x30] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = src[0x38];
                k++;
                value2 = (value2 >> shift) & 0xF;
                value2 |= value1;
                *dst = value2;
            } while (k < 8);
            j++;
            srcStart += 0x40;
        } while (j < 8);
        i++;
        rowBase++;
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
