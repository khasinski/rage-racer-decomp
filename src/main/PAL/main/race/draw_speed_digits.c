#include "common.h"
#include "game/render.h"
#include "game/car.h"

extern u16 g_HudGlyphClut;

u8 *DrawHudDigit(u8 *prim, s32 x, s32 y, s32 digit, u16 clut);
void *GameQueueDrawModePrimWide(void *ot, void *packet, s32 arg2) asm("func_80017390");

void DrawSpeedDigits(s32 x, s32 y, s32 value);
void DrawSpeedDigits(s32 x, s32 y, s32 value) {
    u8 **scratch;
    u8 *prim;
    s32 screenX;
    s32 screenY;
    s32 rawX;
    s32 rawY;
    s32 hundreds;
    s32 tenths;
    s32 tens;
    s32 ones;
    u16 color;

    hundreds = value / 100;
    rawX = g_CarSpec->tachoDigitsX + x;
    rawY = g_CarSpec->tachoDigitsY + y;
    color = g_HudGlyphClut;
    scratch = (u8 **)0x1F800000;
    prim = *scratch;

    tenths = value / 10;
    screenX = (s16)rawX;
    screenY = (s16)rawY;
    tens = tenths - (hundreds * 10);
    ones = value - (tenths * 10);

    prim = DrawHudDigit(prim, screenX, screenY, hundreds, color);
    prim = DrawHudDigit(prim, screenX + 8, screenY, tens, color);
    prim = DrawHudDigit(prim, screenX + 0x10, screenY, ones, color);
    *scratch = GameQueueDrawModePrimWide(g_DrawBuffer + 0xCC, prim, 9);
}
