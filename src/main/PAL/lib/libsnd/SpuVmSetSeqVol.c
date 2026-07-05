#include "common.h"

extern u8 *D_801E79CC[];
extern s16 D_801E4BE6;
extern volatile u8 D_801E42F8;
extern u8 D_8009DF20[];
extern volatile u8 D_8009E0A0[];
extern u8 D_8009E0C6[];

s16 SpuVmSetSeqVol(s32 arg0, u16 arg1, u16 arg2, s16 arg3) asm("func_80076C58");

s16 SpuVmSetSeqVol(s32 arg0, u16 arg1, u16 arg2, s16 arg3) {
    register u32 offset asm("$2");
    register u8 *base asm("$8");
    register u8 *entry asm("$3");
    register u32 index asm("$3");
    register u16 x asm("$9");
    register u16 y asm("$8");

    offset = (arg0 & 0xFF) << 2;
    base = *(u8 **)((u8 *)D_801E79CC + offset);
    __asm__ volatile("addiu $sp,$sp,-8" ::: "memory");
    D_801E4BE6 = arg0;
    arg0 <<= 16;
    index = ((u32)arg0) >> 24;
    offset = index * 0xAC;
    entry = (u8 *)(offset + (s32)base);

    *(u16 *)(entry + 0x74) = arg1;
    offset = *(volatile u16 *)(entry + 0x74);
    arg0 >>= 16;
    *(u16 *)(entry + 0x76) = arg2;
    if (offset >= 0x80) {
        *(u16 *)(entry + 0x74) = 0x7F;
    }

    offset = *(volatile u16 *)(entry + 0x76);
    if (offset >= 0x80) {
        *(u16 *)(entry + 0x76) = 0x7F;
    }

    index = arg1 & 0xFFFF;
    offset = index << 7;
    x = offset + index;
    index = arg2 & 0xFFFF;
    offset = index << 7;
    y = offset + index;

    if (arg3 == 1) {
        register s32 i asm("$6");
        register u8 *dst asm("$7");
        register u8 *dstY asm("$10");
        register s32 signedIndex asm("$5");

        i = 0;
        if (D_801E42F8 != 0) {
            arg0 &= 0xFFFF;
            dst = D_8009DF20;
            dstY = dst + 2;
            do {
                offset = i << 16;
                signedIndex = (s32)offset >> 16;
                offset = signedIndex * 3;
                offset *= 4;
                offset += signedIndex;
                offset *= 4;

                if (*(u16 *)&D_8009E0C6[offset] != arg0) {
                    offset = i + 1;
                } else {
                    offset = signedIndex << 19;
                    offset = (s32)offset >> 15;
                    *(u16 *)(offset + (s32)dst) = x;
                    *(u16 *)(offset + (s32)dstY) = y;
                    offset = D_8009E0A0[signedIndex];
                    offset |= 3;
                    D_8009E0A0[signedIndex] = offset;
                    offset = i + 1;
                }

                i = offset;
                __asm__("" : "=r"(offset) : "0"(offset));
            } while (((s32)(offset << 16) >> 16) < D_801E42F8);
        }
    }

    offset = D_801E4BE6;
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return offset;
}
