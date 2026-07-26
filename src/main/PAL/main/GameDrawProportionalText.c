#include "common.h"

/*
 * Local wide-parameter declarations. Retail passes x / y / clutIndex straight
 * through in full words; the s16 / u16 typing in game/render.h would make gcc
 * insert sign-extends and a truncation here, so this TU declares both the
 * callee and this function with s32 parameters instead of including the header.
 */
void DrawProportionalTextShadedWide(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    s32 intensity) asm("func_80016B7C");
void GameDrawProportionalText(s32 x, s32 y, u8 *str, s32 clutIndex) asm("func_80016EA0");

/* Opaque wrapper over GameDrawProportionalTextShaded: intensity 0x100 selects
 * the raw-texture (SetShadeTex) path instead of a modulated, semi-transparent
 * one. */
void GameDrawProportionalText(s32 x, s32 y, u8 *str, s32 clutIndex) {
    DrawProportionalTextShadedWide(x, y, str, clutIndex, 0x100);
}
