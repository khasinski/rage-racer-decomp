#include "common.h"

s32 func_80034F18(s32 arg0, s32 arg1) {
    s32 quotient;
    s32 quotientPart;

    quotient = arg0 / 25;
    arg0 -= quotient * 25;
    arg0 = (arg0 * 40) + arg1;
    quotientPart = quotient * 1000;
    return quotientPart + arg0;
}
