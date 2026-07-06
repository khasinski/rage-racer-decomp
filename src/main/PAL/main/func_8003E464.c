#include "common.h"

extern u8 *D_801E4448;
extern s32 D_801E4364;
extern volatile s32 D_801E408C;
extern u8 D_801E42FC[];
extern s16 D_801E4308;
extern u8 *D_801E43F4;

s32 func_800632B0(void);

void func_8003E464(void) {
    register u8 *base asm("s0");
    register u8 *out asm("s1");
    register s32 count asm("a0");
    register s32 value asm("v1");
    register s32 index asm("v0");
    register s32 cmp asm("v0");
    register s32 scene0 asm("v1");
    register s32 scene1 asm("v0");
    register u8 *src asm("v1");
    register s32 recordIndex asm("v1");
    register s32 word0 asm("a0");
    register s32 word1 asm("a1");
    register s32 word2 asm("a2");
    register s32 word3 asm("a3");

    base = D_801E4448;
    index = func_800632B0();
    count = D_801E4364;
    value = index % count;
    out = D_801E42FC;
    value++;
    D_801E4308 = value;
    value = (s16)value;

    if (value <= 0) {
        D_801E4308 = (u16)D_801E4364 - 1;
    } else {
        __asm__ volatile("" ::: "memory");
        cmp = count < value;
        if (cmp != 0) {
            D_801E4308 = (u16)D_801E4364;
        }
    }

    scene0 = D_801E408C;
    *(s32 *)(out + 4) = 1;
    scene1 = D_801E408C;
    *(s32 *)(out + 0) = 0;

    src = (u8 *)((scene0 << 5) + (s32)base);
    word0 = *(s32 *)(src + 0x10);
    word1 = *(s32 *)(src + 0x14);
    word2 = *(s32 *)(src + 0x18);
    word3 = *(s32 *)(src + 0x1C);
    *(s32 *)(out + 0x10) = word0;
    *(s32 *)(out + 0x14) = word1;
    *(s32 *)(out + 0x18) = word2;
    *(s32 *)(out + 0x1C) = word3;

    __asm__ volatile("" ::: "memory");
    recordIndex = *(s16 *)((scene1 << 2) + (s32)base + 8);
    *(s32 *)(out + 0x30) = 0;
    index = (recordIndex << 1) + recordIndex;
    index <<= 2;
    index += 0x50;
    D_801E43F4 = base + index;
}
