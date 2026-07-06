#include "common.h"

extern s32 D_8009E6A4;
extern u8 D_8007DAF4[];
extern u8 D_8007DBE4[];
extern u8 D_801C0504[];
extern u8 D_801C0618[];

void func_80032FF0(u8 *arg0, u8 *arg1);

void func_80032D5C(s32 arg0) {
    register u8 *cursor __asm("$16");
    register s32 col __asm("$17");
    register s32 bufferOffset __asm("$18");
    register s32 row __asm("$19");
    register s32 rowOffset __asm("$20");

    if (arg0 != 0) {
        row = 0;
        rowOffset = 0;

nonzero_outer:
        col = 0;
        cursor = D_801C0618;
        bufferOffset = 0;
nonzero_inner:
        {
            register u8 *dst __asm("$4");
            register s32 offset __asm("$2");

            dst = D_801C0504;
            dst = bufferOffset + dst;
            offset = rowOffset + 0x2C;
            dst += offset;
            func_80032FF0(dst, D_8007DAF4 + rowOffset);
        }
        if (D_8009E6A4 == 5 && row == 0xB) {
            cursor[0] += 0xE8;
        }
        cursor += 0x237E8;
        bufferOffset += 0x237E8;
        col++;
        if (col < 2) {
            goto nonzero_inner;
        }
        row++;
        rowOffset += 0x14;
        if (row < 0xC) {
            goto nonzero_outer;
        }
    } else {
        row = 0;
        rowOffset = (s32)D_801C0504;
        bufferOffset = 0;

zero_outer:
        col = 0;
        cursor = (u8 *)rowOffset;
zero_inner:
        func_80032FF0(cursor + (bufferOffset + 0x2C), D_8007DBE4 + bufferOffset);
        cursor += 0x237E8;
        col++;
        if (col < 2) {
            goto zero_inner;
        }
        row++;
        bufferOffset += 0x14;
        if (row < 0xB) {
            goto zero_outer;
        }
    }
}
