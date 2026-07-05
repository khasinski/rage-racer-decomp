#include "common.h"

extern volatile s32 D_801E408C;
extern u8 *D_801E4128;
extern volatile s32 D_801E4330;
extern volatile s32 D_801E4338;
extern volatile s16 D_801E433C;
extern volatile s16 D_801E433E;
extern s32 D_801E4340;
extern s32 D_801E4350;
extern s32 D_801E4354;
extern s32 D_801E4358;
extern u8 *D_801E6C88;

void func_8003EBCC(void) {
    register s32 index0 asm("$2");
    register s32 index1 asm("$3");
    register u8 *base asm("$4");
    register s32 *dst asm("$8");
    register s32 word0 asm("$5");
    register s32 word1 asm("$6");
    register s32 word2 asm("$7");
    register u8 *src asm("$2");
    register u8 *record asm("$4");
    register s32 value asm("$2");

    D_801E433C = 1;
    D_801E4330 = 1;

    index0 = D_801E408C;
    base = D_801E4128;
    index1 = D_801E408C;
    src = (u8 *)((index0 << 5) + (s32)base);
    __asm__ volatile("" ::: "memory");
    dst = &D_801E4340;
    __asm__ volatile("" : "=r"(dst) : "0"(dst), "r"(src) : "memory");

    word0 = *(s32 *)(src + 0x10);
    word1 = *(s32 *)(src + 0x14);
    word2 = *(s32 *)(src + 0x18);
    dst[0] = word0;
    dst[1] = word1;
    dst[2] = word2;
    word0 = *(s32 *)(src + 0x1C);
    dst[3] = word0;
    __asm__ volatile("" ::: "memory");

    index1 = index1 << 2;
    index1 = index1 + (s32)base;
    D_801E433E = 0;
    D_801E4338 = 0;

    index1 = *(s16 *)(index1 + 8);
    value = index1 << 1;
    value += index1;
    value <<= 2;
    value += 0x50;
    record = base + value;

    value = *(s16 *)(record + 0);
    D_801E4350 = value;
    value = *(s16 *)(record + 2);
    D_801E4354 = value;
    value = *(s16 *)(record + 4);
    D_801E6C88 = record;
    D_801E4358 = value;
}
