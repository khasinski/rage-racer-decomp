#include "common.h"

void MemFill(u8 *dst, u8 value, s32 count) asm("func_80068180");

void MemFill(u8 *dst, u8 value, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    if (count != 0) {
        s32 end = -1;

        do {
            *dst = value;
            i--;
            dst++;
        } while (i != end);
    }
}
