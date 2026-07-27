#include "common.h"
#include "game/render.h"
#include "game/car.h"

extern u16 g_HudGlyphClut asm("D_801E4130");

u8 *GameDrawHudDigit(u8 *prim, s32 x, s32 y, s32 digit, u16 clut) asm("func_80033B7C");
void *func_80017390(void *ot, void *packet, s32 arg2);

void GameDrawSpeedDigits(s32 x, s32 y, s32 value) asm("func_80033C18");
void GameDrawSpeedDigits(s32 x, s32 y, s32 value) {
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

    prim = GameDrawHudDigit(prim, screenX, screenY, hundreds, color);
    prim = GameDrawHudDigit(prim, screenX + 8, screenY, tens, color);
    prim = GameDrawHudDigit(prim, screenX + 0x10, screenY, ones, color);
    *scratch = func_80017390(g_DrawBuffer + 0xCC, prim, 9);
}
