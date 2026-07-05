#include "common.h"

extern u32 D_801E6F2C[];

void func_8005D6EC(s32 arg0);

void func_8004B9B8(void) {
    register s32 i asm("$4");
    register u32 *stackPtr asm("$5");
    register u32 *base asm("$6");
    register u32 *cursor asm("$3");
    u32 saved[8];
    register u32 value asm("$2");

    func_8005D6EC(1);

    i = 0;
    stackPtr = saved;
    base = D_801E6F2C;
    cursor = base;
    do {
        value = *cursor;
        cursor++;
        i++;
        *stackPtr = value;
        stackPtr++;
    } while (i < 8);

    i = 0;
    cursor = base;
    do {
        value = cursor[8];
        i++;
        *cursor = value;
        cursor++;
    } while (i < 0x1F8);

    i = 0;
    stackPtr = saved;
    cursor = base;
    do {
        value = *stackPtr;
        stackPtr++;
        i++;
        cursor[0x1F8] = value;
        cursor++;
    } while (i < 8);
}
