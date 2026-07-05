#include "common.h"

extern volatile u8 D_801E42F8;
extern volatile u16 D_801E4BEA;
extern volatile u16 D_801F2A08;
extern volatile u16 D_801F2A0C;
extern volatile u16 D_8009E670;
extern volatile u16 D_8009E674;
extern u8 D_8009E0B8[];
extern u8 D_8009E0BC[];
extern u8 D_8009E0C6[];
extern u8 D_8009E0D3[];

void SpuVmSeqKeyOff(s32 arg0) asm("func_80076ED8");

void SpuVmSeqKeyOff(s32 arg0) {
    register s32 i asm("$7");
    register s32 arg0s asm("$9");
    register volatile u16 *current asm("$10");
    register s32 one asm("$8");
    register s32 index asm("$2");
    register u32 lowMask asm("$6");
    register u32 highMask asm("$5");
    register u32 lowBits asm("$3");
    register u32 highBits asm("$4");
    register u32 bits asm("$2");
    register u32 slot asm("$3");
    register u32 slotIndex asm("$4");
    register s32 offset asm("$3");

    i = 0;
    if (D_801E42F8 != 0) {
        index = arg0 << 16;
        arg0s = index >> 16;
        current = &D_801E4BEA;
        one = 1;
        asm volatile("" : "=r"(i) : "0"(i));
        asm volatile("andi %0,%1,0xff" : "=r"(index) : "r"(i));
        for (;;) {
            asm volatile("1:");
            asm volatile(
                "sll %0,%1,1\n\t"
                "addu %0,%0,%1\n\t"
                "sll %0,%0,2\n\t"
                "addu %0,%0,%1\n\t"
                "sll %0,%0,2"
                : "=&r"(offset)
                : "r"(index));
            index = *(s16 *)&D_8009E0C6[offset];
            if (index != arg0s) {
                goto next;
            }
            index = i & 0xFF;
            *current = index;
            slot = D_801E4BEA;
            asm volatile("andi %0,%1,0xffff" : "=r"(slotIndex) : "r"(slot));
            if (slotIndex < 0x10) {
                lowMask = one << slotIndex;
                highMask = 0;
            } else {
                lowMask = 0;
                index = slotIndex - 0x10;
                highMask = one << index;
            }

            slot = slot & 0xFFFF;
            index = slot * 3;
            index *= 4;
            index += slot;
            index *= 4;
            D_8009E0D3[index] = 0;
            lowBits = D_801F2A08;
            highBits = D_801F2A0C;
            *(s16 *)&D_8009E0BC[index] = 0;
            *(s16 *)&D_8009E0B8[index] = 0;

            bits = D_8009E670;
            asm volatile(
                "or $3,$6,$3\n\t"
                "lui $1,%%hi(D_801F2A08)\n\t"
                "sh $3,%%lo(D_801F2A08)($1)\n\t"
                "nor $3,$0,$3\n\t"
                "and $2,$2,$3\n\t"
                "lui $1,%%hi(D_8009E670)\n\t"
                "sh $2,%%lo(D_8009E670)($1)"
                :
                : "r"(bits), "r"(lowBits), "r"(lowMask)
                : "memory");
            bits = D_8009E674;
            asm volatile(
                "or $4,$5,$4\n\t"
                "lui $1,%%hi(D_801F2A0C)\n\t"
                "sh $4,%%lo(D_801F2A0C)($1)\n\t"
                "nor $4,$0,$4\n\t"
                "and $2,$2,$4\n\t"
                "lui $1,%%hi(D_8009E674)\n\t"
                "sh $2,%%lo(D_8009E674)($1)"
                :
                : "r"(bits), "r"(highBits), "r"(highMask)
                : "memory");
next:
            asm volatile(
                "addiu $7,$7,1\n\t"
                "lui $3,%%hi(D_801E42F8)\n\t"
                "lbu $3,%%lo(D_801E42F8)($3)\n\t"
                "andi $2,$7,0xff\n\t"
                "sltu $2,$2,$3\n\t"
                ".set noreorder\n\t"
                "bnez $2,1b\n\t"
                "andi $2,$7,0xff"
                :
                :
                : "memory");
            break;
        }
    }
}
