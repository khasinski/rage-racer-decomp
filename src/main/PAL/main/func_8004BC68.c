#include "common.h"

extern u32 D_801E6F2C[];

void func_8005D6EC(s32 arg0);

void func_8004BC68(void) {
    register s32 i asm("$8");
    register s32 j asm("$7");
    register s32 mirror asm("$11");
    register u32 *base asm("$12");

    func_8005D6EC(8);
    base = D_801E6F2C;
    i = 0;
    mirror = 0x3F;
    do {
        u8 *cursor;
        s32 leftOffset;
        s32 rightOffset;

        j = 0;
        leftOffset = i << 5;
        rightOffset = (mirror - i) << 5;
        cursor = (u8 *)base;
        do {
            u32 temp;
            u32 *left;
            u32 *right;

            left = (u32 *)(leftOffset + (s32)cursor);
            right = (u32 *)(rightOffset + (s32)cursor);
            cursor += 4;
            temp = *left;
            *left = *right;
            j++;
            *right = temp;
        } while (j < 8);
        i++;
    } while (i < 0x20);
}
