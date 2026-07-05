#include "common.h"

s32 func_80034F18(s32 arg0, s32 arg1) {
    register s32 quotient asm("$3");
    s32 remainder;
    register s32 remainderPart asm("$4");
    s32 quotientPart;

    quotient = arg0 / 25;
    remainder = arg0 - (quotient * 25);
    remainderPart = (remainder * 40) + arg1;
    quotientPart = quotient * 1000;
    return quotientPart + remainderPart;
}
