#include "common.h"

// MASPSX_FLAGS: --store-jump-delay

extern s32 D_801E40AC;
extern volatile u16 D_801E4BEA;
extern volatile u16 D_801F2A08;
extern volatile u16 D_801F2A0C;
extern volatile u16 D_8009E670;
extern volatile u16 D_8009E674;
extern volatile u8 D_8009E0D3[];
extern volatile u8 D_8009E0BC[];
extern volatile u8 D_8009E0B8[];
extern volatile u8 D_8009E0C4[];
extern volatile u8 D_8009E0CA[];
extern volatile u8 D_8009E0CC[];
extern volatile u8 D_8009E0CE[];
extern volatile u16 *D_8009A588;

s32 func_80077A88(s32 voice, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    register s32 one asm("$9") = 1;
    register s32 voiceReg asm("$8");
    register s32 arg4Reg asm("$10") = arg4;
    register s32 highMask asm("$5");
    register s32 lowMask asm("$6");
    register s32 offset asm("$4");
    register s32 index asm("$3");
    register s32 temp asm("$2");
    volatile u16 *spu;
    register u32 activeLow asm("$3");
    register u32 activeHigh asm("$4");
    register u32 bits asm("$2");

    if (D_801E40AC == one) {
        goto fail;
    }

    voiceReg = voice;
    D_801E40AC = one;
    if ((u32)(voiceReg & 0xFFFF) >= 0x18) {
        goto fail_clear;
    }

    index = (s16)voice;
    temp = index << 1;
    temp += index;
    temp <<= 2;
    temp += index;
    offset = temp << 2;

    if (*(s16 *)&D_8009E0CE[offset] != (s16)arg1) {
        goto fail_clear;
    }
    if (*(s16 *)&D_8009E0CA[offset] != (s16)arg2) {
        goto fail_clear;
    }
    if (*(s16 *)&D_8009E0CC[offset] != (s16)arg3) {
        goto fail_clear;
    }
    temp = arg4Reg << 16;
    temp = temp >> 16;
    if (*(s16 *)&D_8009E0C4[offset] != temp) {
        goto fail_clear;
    }

    if (*(s16 *)&D_8009E0B8[offset] == 0xFF) {
        temp = voiceReg & 0xFF;
        index = temp << 1;
        index += temp;
        index <<= 2;
        index += temp;
        index <<= 2;
        D_8009E0D3[index] = 0;
        spu = D_8009A588;
        *(u16 *)&D_8009E0BC[index] = 0;
        spu[0xCA] = 0; spu[0xCB] = 0; } else {
        D_801E4BEA = voiceReg;
        index = D_801E4BEA;
        asm("" : "=r"(index) : "0"(index));
        offset = index & 0xFFFF;
        if ((u32)offset < 16) {
            lowMask = one << offset;
            highMask = 0;
        } else {
            lowMask = 0;
            temp = offset - 16;
            highMask = one << temp;
        }

        index &= 0xFFFF;
        temp = index << 1;
        temp += index;
        temp <<= 2;
        temp += index;
        temp <<= 2;

        D_8009E0D3[temp] = 0;
        activeLow = D_801F2A08;
        activeHigh = D_801F2A0C;
        *(u16 *)&D_8009E0BC[temp] = 0;
        *(u16 *)&D_8009E0B8[temp] = 0;
        bits = D_8009E670;
        asm("or %0,%1,%0" : "=r"(activeLow) : "r"(lowMask), "0"(activeLow));
        D_801F2A08 = activeLow;
        activeLow = ~activeLow;
        bits &= activeLow;
        D_8009E670 = bits;
        bits = D_8009E674;
        asm("or %0,%1,%0" : "=r"(activeHigh) : "r"(highMask), "0"(activeHigh));
        D_801F2A0C = activeHigh;
        activeHigh = ~activeHigh;
        bits &= activeHigh;
        D_8009E674 = bits;
    }

    D_801E40AC = 0;
    return 0;

fail_clear:
    D_801E40AC = 0;
fail:
    return -1;
}
