#include "common.h"
#include "game/state.h"

/* Six 4-byte {x, y} label slots; `labelRow` names one per action. */
typedef struct LabelPoint {
    s16 x;
    s16 y;
} LabelPoint;

extern LabelPoint D_8007C150[];

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

/*
 * The five action captions, each a strip of glyphs lifted from one texture row
 * and placed in the slot `labelRow` assigns it - slot 3 carries two words, so
 * six sprites cover five slots - followed by the black plate and white frame
 * behind each slot and one DR_MODE packet to close the run.
 */
u8 *GameDrawPadConfigLabels(void *ot, u8 *prim, u8 *labelRow) {
    u8 k;
    s32 i;

    k = labelRow[0];
    prim = QueueSpriteTransWide(
        ot, prim, D_8007C150[k].x + 4, D_8007C150[k].y + 8, 0x40, 0x10, 0x40,
        0xBC, 0x7F40);
    k = labelRow[1];
    prim = QueueSpriteTransWide(
        ot, prim, D_8007C150[k].x + 22, D_8007C150[k].y + 8, 0x1C, 0x10, 0x84,
        0xBC, 0x7F40);
    k = labelRow[2];
    prim = QueueSpriteTransWide(
        ot, prim, D_8007C150[k].x + 18, D_8007C150[k].y + 8, 0x28, 0x10, 0x90,
        0xAC, 0x7F40);
    k = labelRow[3];
    prim = QueueSpriteTransWide(
        ot, prim, D_8007C150[k].x + 9, D_8007C150[k].y + 8, 0x18, 0x10, 0x90,
        0xAC, 0x7F40);
    k = labelRow[3];
    prim = QueueSpriteTransWide(
        ot, prim, D_8007C150[k].x + 33, D_8007C150[k].y + 8, 0x20, 0x10, 0xB8,
        0xAC, 0x7F40);
    k = labelRow[4];
    prim = QueueSpriteTransWide(
        ot, prim, D_8007C150[k].x + 5, D_8007C150[k].y + 8, 0x40, 0x10, 0,
        0xBC, 0x7F40);
    i = 0;
    do {
        k = labelRow[i];
        prim = (u8 *)GameAddTilePrim(
            (s32)ot, (s32)prim, D_8007C150[k].x + 1, D_8007C150[k].y + 2, 0x46,
            0x1C, 0, 0, 0);
        k = labelRow[i];
        prim = (u8 *)GameAddTilePrim(
            (s32)ot, (s32)prim, D_8007C150[k].x, D_8007C150[k].y, 0x48, 0x20,
            0xFF, 0xFF, 0xFF);
        i++;
    } while (i < 5);
    return QueueDrawModePrimWide(ot, prim, 0x3B);
}
