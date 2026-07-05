#include "psyq/snd.h"

extern volatile u8 D_801E42F8;
extern volatile u16 D_801E4BEA;
extern volatile u16 D_801F2A08;
extern volatile u16 D_801F2A0C;
extern volatile u16 D_8009E670;
extern volatile u16 D_8009E674;
extern u16 *D_8009A588;
extern u8 D_8009E0B8[];
extern u8 D_8009E0BA[];
extern u8 D_8009E0BC[];
extern u8 D_8009E0BE[];
extern u8 D_8009E0C6[];
extern u8 D_8009E0C8[];
extern u8 D_8009E0CA[];
extern u8 D_8009E0CC[];
extern u8 D_8009E0D3[];

void _SsVmInit(void) {
    register s32 i asm("$8");
    register s32 ff asm("$9");
    register s32 one asm("$10");
    register s32 index asm("$4");
    register s32 offset asm("$2");
    register s32 shifted asm("$4");
    register s32 eighteen asm("$3");
    register u32 lowMask asm("$7");
    register u32 highMask asm("$6");
    register u32 lowBits asm("$3");
    register u32 highBits asm("$4");
    register s32 next asm("$5");
    volatile u16 *spu;
    u32 bits;

    i = 0;
    if (D_801E42F8 != 0) {
        ff = 0xFF;
        one = 1;
        do {
            index = (s16)i;
            offset = index * 0x34;
            eighteen = 0x18;
            shifted = index << 19;
            *(s16 *)&D_8009E0BA[offset] = eighteen;
            *(s16 *)&D_8009E0B8[offset] = ff;
            D_8009E0D3[offset] = 0;
            *(s16 *)&D_8009E0BC[offset] = 0;
            *(s16 *)&D_8009E0BE[offset] = 0;
            *(s16 *)&D_8009E0C6[offset] = ff;
            *(s16 *)&D_8009E0C8[offset] = 0;
            *(s16 *)&D_8009E0CA[offset] = 0;
            *(s16 *)&D_8009E0CC[offset] = ff;

            shifted = shifted >> 15;
            spu = (volatile u16 *)(shifted + (s32)D_8009A588);
            spu[3] = 0x200;
            spu[2] = 0x1000;
            spu[4] = 0x80FF;
            spu[0] = 0;
            spu[1] = 0;
            spu[5] = 0x4000;

            D_801E4BEA = i;
            lowBits = D_801E4BEA;
            __asm__("andi %0,%1,0xffff" : "=r"(index) : "r"(lowBits));
            if ((u32)index < 0x10) {
                lowMask = one << index;
                highMask = 0;
            } else {
                lowMask = 0;
                offset = index - 0x10;
                highMask = one << offset;
            }

            next = i + 1;
            i = next;
            lowBits &= 0xFFFF;
            offset = lowBits * 0x34;
            D_8009E0D3[offset] = 0;
            lowBits = D_801F2A08;
            highBits = D_801F2A0C;
            __asm__ volatile("" ::: "memory");
            next <<= 16;
            *(s16 *)&D_8009E0BC[offset] = 0;
            *(s16 *)&D_8009E0B8[offset] = 0;

            bits = D_8009E670;
            __asm__("or %0,%1,%2" : "=r"(lowBits) : "r"(lowMask), "r"(lowBits));
            D_801F2A08 = lowBits;
            D_8009E670 = bits & ~lowBits;
            bits = D_8009E674;
            __asm__("or %0,%1,%2" : "=r"(highBits) : "r"(highMask), "r"(highBits));
            D_801F2A0C = highBits;
            D_8009E674 = bits & ~highBits;
        } while ((next >> 16) < D_801E42F8);
    }
}
