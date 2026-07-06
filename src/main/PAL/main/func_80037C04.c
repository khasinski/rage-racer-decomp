#include "common.h"

extern s16 D_8009E83C;
extern u8 *D_8019C900;

void *func_80017390(void *ot, void *packet, s32 arg2);
void func_80064DDC(void *ot, void *prim);
void SetShadeTex(u8 *prim, s32 enabled) asm("func_80064EB8");
void SetSprt(u8 *prim) asm("func_80064FA8");

void func_80037C04(void) {
    register u8 *scratch asm("$18");
    register s32 track asm("$22");
    register s32 divisor asm("$19");
    register s32 digitsDrawn asm("$21");
    register s32 xOffset asm("$20");
    register s32 quotient asm("$17");
    register u8 *packet asm("$16");

    scratch = *(u8 **)0x1F800000;
    track = D_8009E83C;
    divisor = 1;
    digitsDrawn = 0;
    xOffset = 0;
    packet = scratch;

    while (1) {
        quotient = track / divisor;
        if (quotient != 0) {
            goto drawDigit;
        }
        if (digitsDrawn > 0) {
            goto done;
        }

drawDigit:
        {
            register s32 y asm("$3");
            register s32 x asm("$2");
            register s32 magic asm("$4");
            register u8 *oldPacket asm("$5");

            SetSprt(scratch);
            SetShadeTex(scratch, 1);

            magic = 0x66666667;
            asm volatile("" : : "r"(magic));
            divisor *= 10;
            y = 0x120 - xOffset;
            xOffset += 0x18;
            digitsDrawn++;
            scratch += 0x14;
            oldPacket = packet;
            asm volatile("" : : "r"(oldPacket));
            x = 0x48;
            asm volatile("mult %0,%1" : : "r"(quotient), "r"(magic));
            packet[0x0D] = x;
            x = 0x18;
            *(s16 *)(packet + 0x10) = x;
            x = 0x20;
            *(s16 *)(packet + 0x12) = x;
            x = 0x10;
            *(s16 *)(packet + 0x0A) = x;
            x = 0x780B;
            *(s16 *)(packet + 0x0E) = x;
            x = quotient >> 31;
            *(s16 *)(packet + 0x08) = y;

            asm volatile("mfhi %0" : "=r"(y));
            y >>= 2;
            y -= x;
            x = (y << 2) + y;
            x <<= 1;
            x = quotient - x;
            y = x << 1;
            y += x;
            y <<= 3;
            packet[0x0C] = y;

            packet += 0x14;
            func_80064DDC(D_8019C900 + 0xCC, oldPacket);
        }
    }

done:
    {
        register void *ot asm("$4");
        register u8 *finalScratch asm("$5");
        register s32 arg2 asm("$6");

        finalScratch = scratch;
        packet = (u8 *)0x1F800000;
        ot = D_8019C900 + 0xCC;
        arg2 = 9;
        *(u8 **)packet = finalScratch;
        *(u8 **)packet = func_80017390(ot, finalScratch, arg2);
    }
}
