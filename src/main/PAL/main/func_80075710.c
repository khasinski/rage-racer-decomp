#include "common.h"

void func_8007B294(s32);
void func_80079B60(s32, u8 *);
void func_80075FA4(void);

extern u8 D_8009E5E0[];

extern volatile u16 D_801E4B5C;
extern volatile u16 D_8019CA68;
extern volatile u16 D_801F17AC;

extern u16 D_8009DF20[];
extern u8 D_8009E0A0[];
extern u8 D_801E4CFC[];

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
extern u8 D_8009E0C0[];
extern u8 D_8009E0C2[];
extern u8 D_8009E0C6[];
extern u8 D_8009E0C8[];
extern u8 D_8009E0CA[];
extern u8 D_8009E0CC[];
extern u8 D_8009E0D3[];
extern u8 D_8009E0D4[];
extern u8 D_8009E0D6[];
extern u8 D_8009E0D8[];
extern u8 D_8009E0DA[];
extern u8 D_8009E0DC[];
extern u8 D_8009E0E0[];
extern u8 D_8009E0E2[];
extern u8 D_8009E0E4[];
extern u8 D_8009E0E6[];
extern u8 D_8009E0E8[];

extern volatile u16 D_8019C780;
extern volatile u16 D_8019C782;
extern volatile u16 D_8009E680;
extern volatile u16 D_8009E684;
extern volatile u32 D_8019C778;
extern volatile u32 D_8019C77C;
extern volatile u8 D_801E4D88;
extern volatile u16 D_801E3FB0;
extern volatile u16 D_801E40D0;

void func_80075710(s32 arg0) {
    register s32 i asm("$5");
    register s32 ff asm("$9");
    register s32 one asm("$8");
    register s32 index asm("$3");
    register s32 offset asm("$2");
    register s32 shifted asm("$4");
    register s32 eighteen asm("$3");
    register s32 mindex asm("$4");
    register u32 lowMask asm("$7");
    register u32 highMask asm("$6");
    register u32 lowBits asm("$3");
    register u32 highBits asm("$4");
    volatile u16 *spu;
    u32 bits;
    u32 n;
    u32 cond;

    {
        u8 *p = D_8009E5E0;
        func_8007B294(0);
        D_801E4B5C = 0;
        D_8019CA68 = 0;
        asm volatile("" ::: "memory");
        func_80079B60(0x20, p);
    }

    for (i = 0; (u16)i < 192; i++) D_8009DF20[(u16)i] = 0;
    for (i = 0; (u16)i < 24; i++) D_8009E0A0[(u16)i] = 0;
    D_801F17AC = 0;
    for (i = 0; (u16)i < 16; i++) D_801E4CFC[(u16)i] = 0;

    n = arg0 & 0xff;
    if (n >= 24) {
        D_801E42F8 = 24;
    } else {
        D_801E42F8 = n;
    }
    if (D_801E42F8 != 0) {
            i = 0;
            ff = 0xFF;
            one = 1;
            do {
                index = (u16)i;
                shifted = index << 3;
                offset = index * 0x34;
                eighteen = 0x18;   *(s16 *)&D_8009E0BA[offset] = eighteen;
                eighteen = -1;     *(s16 *)&D_8009E0C6[offset] = eighteen;
                *(s16 *)&D_8009E0B8[offset] = ff;
                D_8009E0D3[offset] = 0;
                *(s16 *)&D_8009E0BC[offset] = 0;
                *(s16 *)&D_8009E0BE[offset] = 0;
                *(s16 *)&D_8009E0C8[offset] = 0;
                *(s16 *)&D_8009E0CA[offset] = 0;
                *(s16 *)&D_8009E0CC[offset] = ff;
                *(s16 *)&D_8009E0C0[offset] = 0;
                eighteen = 0x40;   D_8009E0C2[offset] = eighteen;
                *(s16 *)&D_8009E0D4[offset] = 0;
                *(s16 *)&D_8009E0D6[offset] = 0;
                *(s16 *)&D_8009E0D8[offset] = 0;
                *(s16 *)&D_8009E0DA[offset] = 0;
                *(s16 *)&D_8009E0E0[offset] = 0;
                *(s16 *)&D_8009E0E2[offset] = 0;
                *(s16 *)&D_8009E0E4[offset] = 0;
                *(s16 *)&D_8009E0E6[offset] = 0;
                *(s16 *)&D_8009E0E8[offset] = 0;
                *(s16 *)&D_8009E0DC[offset] = 0;

                spu = (volatile u16 *)&D_8009A588[(u16)shifted];
                spu[3] = 0x200;
                spu[2] = 0x1000;
                spu[4] = 0x80FF;
                spu[0] = 0;
                spu[1] = 0;
                spu[5] = 0x4000;

                D_801E4BEA = i;
                lowBits = D_801E4BEA;
                __asm__("andi %0,%1,0xffff" : "=r"(mindex) : "r"(lowBits));
                if ((u32)mindex < 0x10) {
                    lowMask = one << mindex;
                    highMask = 0;
                } else {
                    lowMask = 0;
                    offset = mindex - 0x10;
                    highMask = one << offset;
                }

                lowBits &= 0xFFFF;
                offset = lowBits * 0x34;
                D_8009E0D3[offset] = 0;
                lowBits = D_801F2A08;
                highBits = D_801F2A0C;
                __asm__ volatile("");
                i++;
                __asm__ volatile("" ::: "memory");
                *(s16 *)&D_8009E0BC[offset] = 0;
                *(s16 *)&D_8009E0B8[offset] = 0;

                bits = D_8009E670;
                __asm__ volatile("");
                __asm__("or %0,%1,%2" : "=r"(lowBits) : "r"(lowMask), "r"(lowBits));
                __asm__("or %0,%1,%2" : "=r"(highBits) : "r"(highMask), "r"(highBits));
                D_801F2A08 = lowBits;
                bits = bits & ~lowBits;
                __asm__ volatile("");
                D_801F2A0C = highBits;
                D_8009E670 = bits;
                bits = D_8009E674;
                cond = D_801E42F8;
                D_8009E674 = bits & ~highBits;
            } while ((u16)i < cond);
    }

    D_8019C780 = 0x3FFF;
    D_8019C782 = 0x3FFF;
    D_8009E670 = 0;
    D_8009E674 = 0;
    D_801F2A08 = 0;
    D_8009E680 = 0;
    D_8009E684 = 0;
    D_8019C778 = 0;
    D_8019C77C = 0;
    D_801E4D88 = 0;
    D_801E3FB0 = 0;
    D_801E40D0 = 0x80;
    func_80075FA4();
}
