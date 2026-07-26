#include "common.h"
#include "game/state.h"

extern u8 g_PadType asm("D_801E4369");

/* The two 0..7 selections; g_PadType picks which one the screen edits. */
extern s16 D_8019CE08;
extern s16 D_8019CB08;
/* 0 while a controller is present, else the error code the banner reports. */
extern s32 D_801E79C8;

/* "INSERT CONTROLLER" / "CONTROLLER ERROR" */
extern char D_80010000[];
extern char D_80010014[];

extern u8 *g_DrawBuffer asm("D_8019C900");

void DrawProportionalTextWide(
    s32 x,
    s32 y,
    char *str,
    s32 clutIndex) asm("func_80016EA0");
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
u8 *DrawPadConfigSelectorWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 selection) asm("func_80014C80");
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

/*
 * Game mode 7's screen: either the "no controller" banner, or the two nudge
 * arrows (lit only while the selection can still move that way), the selection
 * panel and the diagram for whichever controller is plugged in. The NeGcon
 * variant adds a translucent caption strip under the diagram.
 */
void GameDrawControllerConfigScreen(void) {
    s32 selection;
    s32 leftLit;
    s32 rightLit;
    u8 *ot;
    u8 *prim;

    if (g_PadType == 0x23) {
        selection = D_8019CB08;
    } else {
        selection = D_8019CE08;
    }
    /* The arrows light while the selection can still move that way; the xor
     * is folded into `selection` because retail lets it clobber the loaded
     * value rather than allocating a second register. */
    leftLit = selection != 0;
    selection ^= 7;
    rightLit = selection != 0;
    if (D_801E79C8 != 0) {
        if (D_801E79C8 == 1) {
            DrawProportionalTextWide(0x3A, 0xEA, D_80010000, 0x7812);
        } else {
            DrawProportionalTextWide(0x40, 0xEA, D_80010014, 0x7812);
        }
    } else {
        ot = g_DrawBuffer + 0xCC;
        prim = *(u8 **)0x1F800000;
        prim = DrawLeftArrowWide(ot, prim, 0x28, 0xE0, leftLit);
        prim = DrawRightArrowWide(ot, prim, 0x108, 0xE0, rightLit);
        if (g_PadType == 0x23) {
            prim = DrawPadConfigSelectorWide(ot, prim, 0xF0, 0x28, D_8019CB08);
            prim = GameDrawNegconConfigDiagram(ot, prim);
            prim = QueueSpriteTransWide(
                ot, prim, 0x10, 0x40, 0xD8, 0x10, 0, 0xA8, 0x7F40);
            prim = QueueDrawModePrimWide(ot, prim, 0x3F);
        } else {
            prim = DrawPadConfigSelectorWide(ot, prim, 0xF0, 0x28, D_8019CE08);
            prim = GameDrawPadConfigDiagram(ot, prim);
        }
        *(u8 **)0x1F800000 = prim;
    }
}
