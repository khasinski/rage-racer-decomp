#include "common.h"
/* game/state.h is deliberately not included: it types `selection` as s32,
 * which would drop the sign-extension retail performs. */

/*
 * Retail calls GameQueueShadedSprite with only nine arguments, leaving its
 * tenth (intensity) whatever happened to be in the argument slot. Declared
 * here with the nine it actually passes so the call site matches.
 */
u8 *QueueShadedSpriteNine(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80016F8C");

/* Local wide-parameter views; see GameQueueSprite.c. */
u8 *QueueSpriteTransWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80017138");
u8 *QueueDrawModePrimWide(void *ot, u8 *prim, s32 tpage) asm("func_80017390");
s32 GameAddTilePrim(
    s32 ot,
    s32 prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) asm("func_80032F34");

/* Local declaration: `selection` is a short here - retail loads the argument
 * slot as a word and sign-extends it into the texel offset. */
u8 *GameDrawPadConfigSelector(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 selection) asm("func_80014C80");

/*
 * The framed "CONFIG n" panel at (x, y): the caption strip, then the three
 * digit cells (the middle one steps 8 texels per configuration), then the
 * white frame drawn as four nested tiles.
 */
u8 *GameDrawPadConfigSelector(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 selection) {
    prim = QueueShadedSpriteNine(
        ot, prim, x + 6, y + 8, 0x30, 0xC, 0x78, 0xC0, 0x7F40);
    prim = QueueDrawModePrimWide(ot, prim, 0x3A);
    prim = QueueSpriteTransWide(
        ot, prim, x + 18, y + 32, 8, 0x10, 0x68, 0x28, 0x7F40);
    prim = QueueSpriteTransWide(
        ot, prim, x + 26, y + 32, 8, 0x10, selection * 8 + 80, 0x18, 0x7F40);
    prim = QueueSpriteTransWide(
        ot, prim, x + 34, y + 32, 8, 0x10, 0x68, 0x28, 0x7F40);
    prim = QueueDrawModePrimWide(ot, prim, 0x5B);
    prim = (u8 *)GameAddTilePrim(
        (s32)ot, (s32)prim, x + 1, y + 2, 0x3A, 0x14, 0, 0, 0);
    prim = (u8 *)GameAddTilePrim(
        (s32)ot, (s32)prim, x + 2, y + 26, 0x38, 0x1A, 0xFF, 0xFF, 0xFF);
    prim = (u8 *)GameAddTilePrim(
        (s32)ot, (s32)prim, x + 1, y + 24, 0x3A, 0x1E, 0, 0, 0);
    return (u8 *)GameAddTilePrim(
        (s32)ot, (s32)prim, x, y, 0x3C, 0x38, 0xFF, 0xFF, 0xFF);
}
