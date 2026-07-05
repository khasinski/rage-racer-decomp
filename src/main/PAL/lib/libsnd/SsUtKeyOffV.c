#include "common.h"

extern volatile s32 D_801E40AC;
extern volatile u16 D_801E4BEA;
extern volatile u16 D_801F2A08;
extern volatile u16 D_801F2A0C;
extern volatile u16 D_8009E670;
extern volatile u16 D_8009E674;
extern u8 D_8009E0D3[];
extern u8 D_8009E0BC[];
extern u8 D_8009E0B8[];

s32 SsUtKeyOffV(s32 arg0) asm("func_80078018");

s32 SsUtKeyOffV(s32 arg0) {
    register s32 one asm("$5") = 1;
    register u32 value asm("$2");
    register u32 highBits asm("$6");
    register u32 lowBits asm("$7");
    register s32 offset asm("$3");
    register u32 activeLow asm("$4");
    register u32 activeHigh asm("$5");
    register u32 maskLow asm("$3");
    register u32 maskHigh asm("$3");

    if (D_801E40AC != one) {
        value = arg0 & 0xFFFF;
        asm volatile("" : "=r"(value) : "0"(value));
        D_801E40AC = one;
        if (value < 0x18) {
            register u32 channel asm("$4");
            register u32 masked asm("$3");

            D_801E4BEA = arg0;
            asm volatile(
                "lui %0, %%hi(D_801E4BEA)\n"
                "lhu %0, %%lo(D_801E4BEA)(%0)"
                : "=r"(channel));
            masked = channel & 0xFFFF;
            asm volatile("" : "=r"(masked) : "0"(masked));

            if (masked < 0x10) {
                lowBits = one << masked;
                highBits = 0;
            } else {
                register u32 temp asm("$2");

                lowBits = 0;
                temp = masked - 0x10;
                highBits = one << temp;
            }

            channel &= 0xFFFF;
            offset = ((((channel * 3) * 4) + channel) * 4);
            D_8009E0D3[offset] = 0;
            activeLow = D_801F2A08;
            activeHigh = D_801F2A0C;
            *(u16 *)&D_8009E0BC[offset] = 0;
            *(u16 *)&D_8009E0B8[offset] = 0;

            maskLow = D_8009E670;
            asm volatile("" : "=r"(maskLow) : "0"(maskLow));
            D_801E40AC = 0;
            asm volatile("" ::: "memory");
            activeLow = lowBits | activeLow;
            D_801F2A08 = activeLow;
            D_8009E670 = maskLow & ~activeLow;

            maskHigh = D_8009E674;
            asm volatile("" : "=r"(maskHigh) : "0"(maskHigh));
            activeHigh = highBits | activeHigh;
            D_801F2A0C = activeHigh;
            D_8009E674 = maskHigh & ~activeHigh;
            asm volatile("" ::: "memory");
            return 0;
        }
        D_801E40AC = 0;
    }
    return -1;
}
