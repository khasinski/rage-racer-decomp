#include "common.h"

extern u8 D_8007DF04[];

void func_80016754(s32 x, s32 y, void *str, s32 color);

void func_80033D50(s32 x, s32 y, s32 value, s32 color, s32 divisor) {
    register s32 savedX asm("$9");
    register s32 savedY asm("$10");
    register s32 savedColor asm("$11");
    register s32 localDivisor asm("$4");
    register s32 whole asm("$7");
    register s32 fraction asm("$5");
    register s32 minutes asm("$3");
    register s32 seconds asm("$7");
    register s32 secondTens asm("$4");
    register s32 fractionHundreds asm("$6");
    register s32 fractionTens asm("$3");
    s32 remainder;

    savedX = x;
    savedY = y;
    localDivisor = divisor;
    savedColor = color;
    if (value >= 0) {
        whole = value / localDivisor;
        remainder = value % localDivisor;

        minutes = whole / 60;
        fraction = (remainder * 1000) / localDivisor;
        seconds = whole % 60;
        D_8007DF04[0] = minutes + '0';

        secondTens = seconds / 10;
        D_8007DF04[2] = secondTens + '0';
        D_8007DF04[3] = (seconds - (secondTens * 10)) + '0';

        fractionHundreds = fraction / 100;
        D_8007DF04[5] = fractionHundreds + '0';

        fractionTens = fraction / 10;
        D_8007DF04[6] = (fractionTens - (fractionHundreds * 10)) + '0';
        D_8007DF04[7] = (fraction - (fractionTens * 10)) + '0';
    } else {
        D_8007DF04[0] = '-';
        D_8007DF04[2] = '-';
        D_8007DF04[3] = '-';
        D_8007DF04[5] = '-';
        D_8007DF04[6] = '-';
        D_8007DF04[7] = '-';
    }

    func_80016754(savedX, savedY, D_8007DF04, savedColor);
}
