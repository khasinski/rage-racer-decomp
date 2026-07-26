#include "common.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} ColorTriplet;

typedef struct {
    ColorTriplet colors[18];
} ColorTable;

extern ColorTable D_800119A0;
extern s32 D_8009B290;
extern s32 g_MenuAltLayout asm("D_8019CB0C");

s32 func_80068568(s32 angle);
void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha);
void func_80047460(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, u8 r, u8 g, u8 b, u8 alpha);

s32 func_8004F048(s32 *counter, s32 step, s32 index) {
    ColorTable localTable;
    void *ot;
    s32 progress;
    s32 wideOffset;
    s32 phase;
    ColorTable *new_var;
    s32 yBase;
    s32 xBase;
    u8 sineByte;
    u16 xBaseHalf;
    s32 yBorder;
    s32 yTop;
    s32 xFocus;
    s32 xFocusOffset;
    s32 sineColor;
    s32 xOffset;
    s32 i;
    s32 colorIndex;
    s32 next;
    ColorTable *srcTable;
    ColorTriplet *color;

    ot = *(void **)0x1F800004;
    srcTable = &D_800119A0;
    new_var = srcTable;
    localTable = *new_var;

    if (step < 0) {
        *counter += step;
        if (*counter < 0) {
            *counter = 0;
        }
    }

    progress = *counter - 11;
    wideOffset = (g_MenuAltLayout != 0) ? 0x2C : 0;

    if (progress >= 0) {
        if (progress >= 11) {
            progress = 10;
        }

        phase = D_8009B290 * 2;
        xBase = 0x9E - wideOffset;
        yBase = ((u32)-(progress * 480)) >> 5;
        xBaseHalf = xBase;
        yBorder = yBase + 0x20E;
        xFocusOffset = index * 8;
        xFocusOffset -= 2;
        xFocus = xBase + xFocusOffset;
        yTop = yBase + 0x20B;

        sineColor = func_80068568(phase % 0x1000);
        if (sineColor < 0) {
            sineColor += 0x3F;
        }
        sineColor = (sineColor >> 6) - 0x41;

        D_8009B290 += 0x20;

        func_80047460(ot, (s16)xFocus, (s16)yTop, 0xD, 0x1A, 0,
                      sineByte = (u8)sineColor, 0, 0xFF);

        color = &localTable.colors[index];
        func_80047024(ot, (s16)(xFocus + 1), (s16)(yBase + 0x20D), 0xB, 0x16, color->r,
                      color->g, color->b, 0xFF);

        func_80047460(ot, xBase, (s16)yBorder, 0x92, 0x14, 0xB4, 0xB4, 0xB4, 0xFF);

        xOffset = 0;
        i = 0;
        colorIndex = 1;
        do {
            func_80047024(ot, (s16)(xBaseHalf + colorIndex), (s16)(yBase + 0x210), 8,
                          0x10, ((ColorTriplet *)((u8 *)localTable.colors + i))->r,
                          ((ColorTriplet *)((u8 *)localTable.colors + i))->g,
                          ((ColorTriplet *)((u8 *)localTable.colors + i))->b, 0xFF);
            i += 3;
            colorIndex += 8;
            xOffset++;
        } while (xOffset < 18);
    }

    if (step >= 0) {
        next = step + *counter;
        if (next >= 25) {
            *counter = 25;
            return 1;
        }
        *counter = next;
    }

    return 0;
}
