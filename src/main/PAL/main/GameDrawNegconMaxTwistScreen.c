#include "common.h"
#include "game/state.h"

/*
 * Four {u, v} texel pairs. The screen copies them into a local and never reads
 * it back - leftover from the steer-play screen, which has its own template
 * eight bytes earlier. Spelled as a struct copy from the existing rodata
 * symbol rather than a brace initialiser so that the template is not emitted a
 * second time; the generated block move is identical.
 */
typedef struct NegconUvTemplate {
    u8 uv[8];
} NegconUvTemplate;

extern NegconUvTemplate D_80010084;

/* "Maximum twist." */
extern char D_8001008C[];

extern u8 *g_DrawBuffer asm("D_8019C900");
/* The 0..3 maximum-twist setting this screen edits. */
extern s16 D_801E418C;

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
 * Game mode 11's overlay: the caption, the two nudge arrows (lit only while
 * the setting can still move that way), the gauge sprite whose width and texel
 * column follow the 0..3 setting, its end cap, and the framed panel.
 */
void GameDrawNegconMaxTwistScreen(void) {
    NegconUvTemplate unused;
    u8 *ot;
    u8 *prim;
    s32 xoff;
    s32 w;

    unused = D_80010084;
    func_80027874(0x18, 0x30, D_8001008C, 0x7F81);
    ot = g_DrawBuffer + 0xCC;
    prim = *(u8 **)0x1F800000;
    prim = DrawLeftArrowWide(ot, prim, 0x28, 0xE0, D_801E418C != 0);
    prim = DrawRightArrowWide(ot, prim, 0x108, 0xE0, D_801E418C != 3);
    if (D_801E418C == 3) {
        xoff = 0;
        w = 0x24;
    } else {
        xoff = 0xC;
        w = 0x18;
    }
    prim = QueueSpriteTransWide(
        ot, prim, xoff + 0x88, 0x30, w, 0x18, D_801E418C * 24, 0x30, 0x7F81);
    prim = QueueSpriteTransWide(ot, prim, 0xAC, 0x30, 4, 0x18, 0x78, 0x30, 0x7F81);
    prim = QueueDrawModePrimWide(ot, prim, 0x3F);
    prim = (u8 *)GameAddTilePrim(
        (s32)ot, (s32)prim, 0, 0x28, 0x124, 0x40, 0, 0, 0);
    *(s32 *)0x1F800000 = GameAddTilePrim(
        (s32)ot, (s32)prim, 0, 0x26, 0x125, 0x44, 0xFF, 0xFF, 0xFF);
}
