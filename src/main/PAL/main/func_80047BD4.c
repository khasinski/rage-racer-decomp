#include "common.h"

void func_80046A2C(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 r,
    s32 g,
    s32 b,
    s32 clut,
    s32 arg11,
    s32 arg12,
    s32 flags);
void *func_80017390(void *ot, void *prim, s32 count);

s32 GameDrawNumber(
    s32 x,
    s16 y,
    s32 flags,
    u32 value,
    u8 r,
    u8 g,
    u8 b,
    u16 clut,
    u8 primitiveCount) asm("func_80047BD4");
s32 GameDrawNumber(
    s32 x,
    s16 y,
    s32 flags,
    u32 value,
    u8 r,
    u8 g,
    u8 b,
    u16 clut,
    u8 primitiveCount) {
    u8 digits[11];
    u16 drawVValue;
    void *ot;
    s32 width;
    s32 height;
    s32 v;
    s32 i;
    s32 digit;
    s32 drawn;
    s32 drawWidth;
    s32 drawHeight;
    s32 newHeight;
    s32 drawV;
    s32 small;
    s32 nextX;

    i = 9;
    if (flags & 8) {
        ot = (u8 *)*(void **)0x1F800004 + 4;
    } else {
        ot = *(void **)0x1F800004;
    }

    height = 16;
    if (flags & 4) {
        width = 8;
        v = 0xDC;
    } else {
        small = flags & 1;
        width = small ? 8 : 6;
        height = small ? 16 : 12;
        v = (-small) & 0x18;
    }

    for (digit = 0; digit <= i; digit++) {
        digits[i - digit] = value % 10;
        value /= 10;
    }

    digits[10] = 0xFF;
    i = 0;
    while ((digits[i] == 0) && (digits[i + 1] != 0xFF)) {
        digits[i] = ' ';
        i++;
    }

    drawn = 0;
    if (flags & 2) {
        i = 0;
    }

    if (digits[i] != 0xFF) {
        drawWidth = width;
        drawHeight = newHeight = height;
        drawV = v;
        while (digits[i] != 0xFF) {
            drawVValue = drawV;
            if (digits[i] == ' ') {
                nextX = x + width;
                if (nextX != drawWidth) {
                    x = nextX;
                } else {
                    x = nextX;
                }
                i++;
                continue;
            }

            func_80046A2C(
                ot,
                (s16)x,
                y,
                drawWidth,
                drawHeight,
                digits[i] * drawWidth,
                drawVValue,
                r,
                g,
                b,
                clut,
                0,
                1,
                0x80);
            i++;
            digit = width;
            drawn++;
            do {
                nextX = x + digit;
                if (nextX) {
                    x = nextX;
                } else {
                    x = nextX;
                }
            } while (0);
        }
    }

    *(void **)0x1F800000 =
        func_80017390(ot, *(void **)0x1F800000, primitiveCount + 27);
    return drawn;
}
