#include "common.h"

extern u32 D_801E6F2C[];

void func_8005D6EC(s32 arg0);

void func_8004BA50(void) {
    s32 i;
    u32 *newPtr;
    u32 *stackPtr;
    u32 *base;
    u32 *cursor;
    u32 saved[8];
    u32 value;

    func_8005D6EC(1);

    i = 0;
    stackPtr = saved;
    base = D_801E6F2C;
    cursor = base;
    do {
        value = cursor[0x1F8];
        cursor++;
        i++;
        *stackPtr = value;
        stackPtr++;
    } while (i < 8);

    i = 0x1F7;
    newPtr = base + 0x1F7;
    cursor = newPtr;
    do {
        value = *cursor;
        i--;
        cursor[8] = value;
        cursor--;
    } while (i >= 0);

    i = 0;
    stackPtr = base;
    cursor = saved;
    do {
        value = *cursor;
        cursor++;
        i++;
        newPtr = stackPtr;
        *newPtr = value;
        stackPtr++;
    } while (8 > i);
}
