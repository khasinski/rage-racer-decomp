#include "common.h"

extern s16 D_8009E834;
extern u8 *D_8019C900;

void func_80033230(void) {
    register u8 *base asm("$4");
    register s32 value asm("$5");
    register u8 *left asm("$7");
    register u8 *right asm("$6");
    s16 tile;

    {
        register s32 leftOffset asm("$2");
        register s32 rightOffset asm("$3");

        leftOffset = 0x237AC;
        rightOffset = 0x237C0;
        base = D_8019C900;
        value = D_8009E834;
        left = base + leftOffset;
        right = base + rightOffset;
    }

    if (value >= 10) {
        *(u8 *)(base + 0x237B8) = 0x18;
    } else {
        *(u8 *)(base + 0x237B8) = 0;
    }

    {
        register s32 quotient asm("$3");
        register s32 digit asm("$3");

        quotient = value / 10;
        asm volatile("" : "=r"(quotient) : "0"(quotient));
        digit = (value - quotient * 10) * 24;
        *(u8 *)(right + 0xC) = digit;
    }

    {
        register s32 cond asm("$2");

        cond = value < 4;
        if (cond != 0) {
            tile = 0x780B;
        } else {
            asm volatile("" ::: "$2");
            tile = 0x780E;
        }
    }

    *(s16 *)(left + 0xE) = tile;
    *(s16 *)(right + 0xE) = tile;
}
