#include "common.h"

extern s32 D_801E408C;
extern u8 *D_801E4150;
extern u8 D_801F18C4[];
extern u8 D_801F198C[];
extern u8 D_801F198E[];

void func_80039644(void) {
    register s32 one asm("t2") = 1;
    register s32 offset asm("a2") = 0;
    register u8 *base asm("t0");
    register s32 scene asm("v1");
    register s32 product asm("v0");
    register s32 baseOffset asm("t1");
    register s32 index asm("a0");
    register s32 tableOffset asm("a1");
    register s32 raw asm("v0");
    register s32 target asm("a3");
    register s32 value asm("v1");

    scene = D_801E408C;
    base = D_801E4150;
    product = (scene << 3) + scene;
    baseOffset = product << 6;

outer:
    __asm__ volatile("" ::: "memory");
    index = 0;
    raw = *(s32 *)(D_801F18C4 + offset);
    tableOffset = baseOffset;
    *(s16 *)(D_801F198E + offset) = one;
    target = raw >> 4;

inner:
    value = *(s16 *)(base + tableOffset + 0x474);
    if (target >= value) {
        *(s16 *)(D_801F198C + offset) = index;
        offset += 0x19C;
        goto next;
    }
    if (value == -1) {
        *(s16 *)(D_801F198C + offset) = 0;
        offset += 0x19C;
        goto next;
    }
    __asm__ volatile("" ::: "memory");
    index++;
    if (index < 0x30) {
        tableOffset += 0xC;
        goto inner;
    }
    offset += 0x19C;

next:
    if (offset < 0x11B4) {
        goto outer;
    }
}
