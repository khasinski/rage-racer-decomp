#include "common.h"
#include "game/menu_types.h"
#include "game/vector.h"
#include "game/render.h"
void GameDrawMenuButton(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7,
                   s32 a8, s32 a9, s32 a10);
void DrawRectOutline(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, u8 r, u8 g, u8 b, u8 alpha);

extern PaintColorTable g_PaintColorTable;
extern s32 D_8009B290;
extern s32 g_MenuAltLayout;

s32 rsin(s32 angle);
void DrawSolidRect(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha);

/* The 18-swatch PAINT COLOR strip with its selection frame and enlarged preview. */
s32 DrawPaintColorPalette(s32 *counter, s32 step, s32 index) {
    PaintColorTable localTable;
    void *ot;
    s32 progress;
    s32 wideOffset;
    s32 phase;
    PaintColorTable *new_var;
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
    PaintColorTable *srcTable;
    Rgb *color;

    ot = *(void **)0x1F800004;
    srcTable = &g_PaintColorTable;
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

        sineColor = rsin(phase % 0x1000);
        if (sineColor < 0) {
            sineColor += 0x3F;
        }
        sineColor = (sineColor >> 6) - 0x41;

        D_8009B290 += 0x20;

        DrawRectOutline(ot, (s16)xFocus, (s16)yTop, 0xD, 0x1A, 0,
                      sineByte = (u8)sineColor, 0, 0xFF);

        color = &localTable.colors[index];
        DrawSolidRect(ot, (s16)(xFocus + 1), (s16)(yBase + 0x20D), 0xB, 0x16, color->r,
                      color->g, color->b, 0xFF);

        DrawRectOutline(ot, xBase, (s16)yBorder, 0x92, 0x14, 0xB4, 0xB4, 0xB4, 0xFF);

        xOffset = 0;
        i = 0;
        colorIndex = 1;
        do {
            DrawSolidRect(ot, (s16)(xBaseHalf + colorIndex), (s16)(yBase + 0x210), 8,
                          0x10, ((Rgb *)((u8 *)localTable.colors + i))->r,
                          ((Rgb *)((u8 *)localTable.colors + i))->g,
                          ((Rgb *)((u8 *)localTable.colors + i))->b, 0xFF);
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

extern s32 D_8007FB2C;

void GameDrawSpriteWide(void *a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7,
                   s32 a8, s32 a9, s32 a10, s32 a11, s32 a12, s32 a13) asm("DrawSprite");

void DrawOwnedCarCounter(s32 arg0, s32 arg1) {
    s32 count;
    s32 a1v;
    void *ot;
    s32 v0, v1, t, t2;
    s16 y;

    count = arg0;
    ot = *(void **)0x1F800004;
    a1v = arg1;

    if (count == 0) {
        D_8007FB2C = 0;
        return;
    }
    if (count < 0) {
        v0 = count + D_8007FB2C;
        D_8007FB2C = v0;
        if (v0 < 0) {
            D_8007FB2C = 0;
        }
    }
    v0 = D_8007FB2C;
    v1 = v0 - 11;
    if (v1 >= 0 && g_MenuAltLayout == 0) {
        if (v1 >= 11) {
            v1 = 10;
        }
        t = ((u32)(v1 * -1120)) >> 5;
        y = t + 0x21B;
        t2 = t + 0x211;
        GameDrawNumber(0x2C, y, 7, a1v, 0x7F, 0x7F, 0x7F, 0x259, 0x20);
        GameDrawNumber(0x44, y, 7, 0xD, 0x7F, 0x7F, 0x7F, 0x259, 0x20);
        GameDrawSpriteWide(ot, 0x17, y, 0x34, 0x10, 0x8C, 0x8C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        GameDrawSpriteWide(ot, 0x7C, y, 0x8, 0x10, 0x8C, 0xDC, 0, 0, 0, 0x259, 1, 1, 0x3B);
        GameDrawMenuButton(0, (s16)t2, 0x99, 0x23, 0, 0, 0, 0, 0, 0, 0);
    }
    if (count > 0) {
        v0 = count + D_8007FB2C;
        D_8007FB2C = v0;
        if (v0 >= 0x1A) {
            D_8007FB2C = 0x19;
        }
    }
}
