#include "common.h"

extern u8 D_8007F6E8[];
extern s32 D_8007FB00;
extern s32 D_8009E694;

void func_80046A2C(
    void *arg0,
    s32 arg1,
    s32 arg2,
    s32 arg3,
    u16 arg4,
    u16 arg5,
    u16 arg6,
    u8 arg7,
    u8 arg8,
    u8 arg9,
    u16 arg10,
    s32 arg11,
    s32 arg12,
    s32 arg13);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void func_80047E60(s32 arg0) {
    void *ot = *(void **)0x1F800004;
    u8 *row = D_8007F6E8;
    s32 y;
    s32 outer;
    s32 one;
    s32 x;
    s32 bit;
    s32 offset;

    if (arg0 == 0) {
        return;
    }

    if (arg0 < 0) {
        if ((D_8009E694 % 6U) == 0) {
            D_8007FB00 += 8;
        }

        offset = D_8007FB00;
        offset += (s32)D_8007F6E8;
        if (*(u8 *)(offset + 7) != 0) {
            D_8007FB00 = 0x10;
        }
        row = (u8 *)(D_8007FB00 + (s32)D_8007F6E8);
    } else {
        row += (arg0 - 1) * 8;
    }

    y = 0x150;
    outer = 0;
    one = 1;
    do {
        x = 0x22;
        bit = 0;
        do {
            if (((*row << bit) & 0x80) != 0) {
                func_80046A2C(
                    (u8 *)ot + 4,
                    (s16)x,
                    (s16)y,
                    4,
                    8,
                    0xFC,
                    0,
                    0,
                    0,
                    0,
                    0x244,
                    one,
                    one,
                    0x80);
            }
            bit++;
            x += 4;
        } while (bit < 8);
        y += 8;
        outer++;
        row++;
    } while (outer < 6);

    outer = 0;
    x = 1;
    bit = 0x4C0000;
    do {
        func_80046A2C(
            (u8 *)ot + 4,
            (s16)(bit >> 16),
            0x33,
            4,
            8,
            0xFC,
            0,
            0,
            0,
            0,
            0x244,
            x,
            x,
            0x80);
        bit += 0x50000;
        outer++;
    } while (outer < 0x10);

    *(void **)0x1F800000 = func_80017390((u8 *)ot + 4, *(void **)0x1F800000, 0x39);
}
