#include "common.h"
#include "game/state.h"

/* Screen positions, six 4-byte {x, y} rows each: where a label sits and where
 * the button it names sits. */
typedef struct CalloutPoint {
    s16 x;
    s16 y;
} CalloutPoint;

extern CalloutPoint D_8007C168[];
extern CalloutPoint D_8007C180[];

/* Screen animation counter; the callouts only draw while it is settled. */
extern s32 D_801E8AA4;

/* Local wide-parameter view of GameQueueLine; see GameQueueLine.c. */
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

/*
 * The five green callout lines joining each action label to its button: one
 * vertical drop from the label, then a two-pixel-thick horizontal run to the
 * button. Suppressed while the panel is still sliding.
 */
u8 *GameDrawPadConfigCallouts(void *ot, u8 *prim, u8 *labelRow, u8 *buttonRow) {
    s32 i;

    if (D_801E8AA4 > -16 && D_801E8AA4 < 16) {
        i = 0;
        do {
            CalloutPoint *lp = &D_8007C168[labelRow[i]];
            CalloutPoint *bp = &D_8007C180[buttonRow[i]];

            prim = QueueLineWide(
                ot, prim, lp->x, lp->y, lp->x, bp->y, 0x20, 0xFF, 0x20);
            prim = QueueLineWide(
                ot, prim, lp->x, bp->y, bp->x, bp->y, 0x20, 0xFF, 0x20);
            prim = QueueLineWide(
                ot, prim, lp->x, bp->y - 1, bp->x, bp->y - 1, 0x20, 0xFF, 0x20);
            i++;
        } while (i < 5);
    }
    return prim;
}
