#include "common.h"
#include "game/vector.h"
#include "game/state.h"

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
    s32 clutIndex) asm("GameQueueSpriteTrans");
u8 *QueueDrawModePrimWide(void *ot, u8 *prim, s32 tpage) asm("QueueDrawModePrim");
s32 AddTilePrim(
    s32 ot,
    s32 prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b);

/*
 * The five action captions, each a strip of glyphs lifted from one texture row
 * and placed in the slot `labelRow` assigns it - slot 3 carries two words, so
 * six sprites cover five slots - followed by the black plate and white frame
 * behind each slot and one DR_MODE packet to close the run.
 */
u8 *DrawPadConfigLabels(void *ot, u8 *prim, u8 *labelRow) {
    u8 k;
    s32 i;

    k = labelRow[0];
    prim = QueueSpriteTransWide(
        ot, prim, g_PadLabelSlots[k].vx + 4, g_PadLabelSlots[k].vy + 8, 0x40, 0x10, 0x40,
        0xBC, 0x7F40);
    k = labelRow[1];
    prim = QueueSpriteTransWide(
        ot, prim, g_PadLabelSlots[k].vx + 22, g_PadLabelSlots[k].vy + 8, 0x1C, 0x10, 0x84,
        0xBC, 0x7F40);
    k = labelRow[2];
    prim = QueueSpriteTransWide(
        ot, prim, g_PadLabelSlots[k].vx + 18, g_PadLabelSlots[k].vy + 8, 0x28, 0x10, 0x90,
        0xAC, 0x7F40);
    k = labelRow[3];
    prim = QueueSpriteTransWide(
        ot, prim, g_PadLabelSlots[k].vx + 9, g_PadLabelSlots[k].vy + 8, 0x18, 0x10, 0x90,
        0xAC, 0x7F40);
    k = labelRow[3];
    prim = QueueSpriteTransWide(
        ot, prim, g_PadLabelSlots[k].vx + 33, g_PadLabelSlots[k].vy + 8, 0x20, 0x10, 0xB8,
        0xAC, 0x7F40);
    k = labelRow[4];
    prim = QueueSpriteTransWide(
        ot, prim, g_PadLabelSlots[k].vx + 5, g_PadLabelSlots[k].vy + 8, 0x40, 0x10, 0,
        0xBC, 0x7F40);
    i = 0;
    do {
        k = labelRow[i];
        prim = (u8 *)AddTilePrim(
            (s32)ot, (s32)prim, g_PadLabelSlots[k].vx + 1, g_PadLabelSlots[k].vy + 2, 0x46,
            0x1C, 0, 0, 0);
        k = labelRow[i];
        prim = (u8 *)AddTilePrim(
            (s32)ot, (s32)prim, g_PadLabelSlots[k].vx, g_PadLabelSlots[k].vy, 0x48, 0x20,
            0xFF, 0xFF, 0xFF);
        i++;
    } while (i < 5);
    return QueueDrawModePrimWide(ot, prim, 0x3B);
}

/* Local wide-parameter view of GameQueueLine; see GameQueueSprite.c. */
u8 *QueueLineWide(
    void *ot,
    u8 *prim,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 r,
    s32 g,
    s32 b) asm("GameQueueLine");

/*
 * The five green callout lines joining each action label to its button: one
 * vertical drop from the label, then a two-pixel-thick horizontal run to the
 * button. Suppressed while the panel is still sliding.
 */
u8 *DrawPadConfigCallouts(void *ot, u8 *prim, u8 *labelRow, u8 *buttonRow) {
    s32 i;

    if (g_ControllerSceneAngleY > -16 && g_ControllerSceneAngleY < 16) {
        i = 0;
        do {
            DVec *lp = &g_PadCalloutLabelPoints[labelRow[i]];
            DVec *bp = &g_PadCalloutButtonPoints[buttonRow[i]];

            prim = QueueLineWide(
                ot, prim, lp->vx, lp->vy, lp->vx, bp->vy, 0x20, 0xFF, 0x20);
            prim = QueueLineWide(
                ot, prim, lp->vx, bp->vy, bp->vx, bp->vy, 0x20, 0xFF, 0x20);
            prim = QueueLineWide(
                ot, prim, lp->vx, bp->vy - 1, bp->vx, bp->vy - 1, 0x20, 0xFF, 0x20);
            i++;
        } while (i < 5);
    }
    return prim;
}

extern s16 g_PadMappingIndex;

/* One whole standard-pad diagram for the current selection: the five action
 * labels, then the five callout lines from each label to its button. */
u8 *DrawPadConfigDiagram(void *ot, u8 *prim) {
    prim = DrawPadConfigLabels(ot, prim, &g_PadConfigLabelRows[g_PadMappingIndex * 5]);
    return DrawPadConfigCallouts(
        ot, prim, &g_PadConfigLabelRows[g_PadMappingIndex * 5], &g_PadConfigButtonRows[g_PadMappingIndex * 5]);
}

extern s16 g_NegconMappingIndex;

/* One whole NeGcon diagram for the current selection: labels, then callouts. */
u8 *DrawNegconConfigDiagram(void *ot, u8 *prim) {
    prim = DrawPadConfigLabels(ot, prim, &g_NegconConfigLabelRows[g_NegconMappingIndex * 5]);
    return DrawPadConfigCallouts(
        ot, prim, &g_NegconConfigLabelRows[g_NegconMappingIndex * 5], &g_NegconConfigButtonRows[g_NegconMappingIndex * 5]);
}
