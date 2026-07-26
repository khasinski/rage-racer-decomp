#include "common.h"
#include "game/state.h"

/*
 * Four {u, v} texel pairs copied into a local that is never read back; see
 * GameDrawNegconMaxTwistScreen.c for why this is spelled as a struct copy.
 */
typedef struct NegconUvTemplate {
    u8 uv[8];
} NegconUvTemplate;

extern NegconUvTemplate D_80010070;

/* "Steer play." */
extern char D_80010078[];

extern u8 *g_DrawBuffer asm("D_8019C900");
/* The 0..3 steering-play setting this screen edits. */
extern s16 D_8019CAD0;
/* Play in hundredths of a degree per setting; the gauge marks scale from it. */
extern s16 D_8007C260[];

void func_80027874(s32 x, s32 y, char *str, s32 clutIndex);

/* Local wide-parameter views; see GameQueueSprite.c / GameDrawLeftArrow.c. */
u8 *DrawLeftArrowWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 pulse) asm("func_80014A60");
u8 *DrawRightArrowWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 pulse) asm("func_80014B70");
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
u8 *QueueLineWide(
    void *ot,
    u8 *prim,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 r,
    s32 g,
    s32 b) asm("func_800172D4");
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

/*
 * Game mode 10's overlay: the caption, the two nudge arrows, the three digit
 * cells of the setting, the framed panel, and the play gauge - two green rules
 * either side of the white centre line, spaced by the selected play scaled
 * into pixels.
 */
void GameDrawNegconSteerPlayScreen(void) {
    NegconUvTemplate unused;
    u8 *ot;
    u8 *prim;
    s32 span;
    s32 y;

    unused = D_80010070;
    func_80027874(0x18, 0x30, D_80010078, 0x7F81);
    ot = g_DrawBuffer + 0xCC;
    prim = *(u8 **)0x1F800000;
    prim = DrawLeftArrowWide(ot, prim, 0x28, 0xE0, D_8019CAD0 != 0);
    prim = DrawRightArrowWide(ot, prim, 0x108, 0xE0, D_8019CAD0 != 3);
    prim = QueueSpriteTransWide(
        ot, prim, 0x70, 0x30, 0xC, 0x18, 0x8C, 0x18, 0x7F81);
    prim = QueueSpriteTransWide(
        ot, prim, 0x7C, 0x30, 0xC, 0x18, D_8019CAD0 * 12 + 152, 0x18, 0x7F81);
    prim = QueueSpriteTransWide(
        ot, prim, 0x88, 0x30, 0xC, 0x18, 0x6C, 0x30, 0x7F81);
    prim = QueueDrawModePrimWide(ot, prim, 0x3F);
    prim = (u8 *)GameAddTilePrim(
        (s32)ot, (s32)prim, 0, 0x28, 0x124, 0x40, 0, 0, 0);
    prim = (u8 *)GameAddTilePrim(
        (s32)ot, (s32)prim, 0, 0x26, 0x125, 0x44, 0xFF, 0xFF, 0xFF);
    span = ((D_8007C260[D_8019CAD0] << 7) / 100) * 2;
    y = 230 - span;
    prim = QueueLineWide(ot, prim, 0x94, y, 0xA8, y, 0x20, 0x40, 0xFF);
    prim = QueueLineWide(ot, prim, 0x94, y + 1, 0xA8, y + 1, 0x20, 0x40, 0xFF);
    prim = QueueLineWide(
        ot, prim, 0x94, span + 230, 0xA8, span + 230, 0x20, 0x40, 0xFF);
    span = span + 231;
    prim = QueueLineWide(ot, prim, 0x94, span, 0xA8, span, 0x20, 0x40, 0xFF);
    prim = QueueLineWide(ot, prim, 0x94, 0xE6, 0xA8, 0xE6, 0, 0, 0);
    *(u8 **)0x1F800000 =
        QueueLineWide(ot, prim, 0x94, 0xE7, 0xA8, 0xE7, 0, 0, 0);
}
