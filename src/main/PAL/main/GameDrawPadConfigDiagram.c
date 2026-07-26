#include "common.h"
#include "game/state.h"

/* Eight rows of five bytes: the label slot each action sits in, and the button
 * each label points at, for the standard pad. */
extern u8 D_8007C1C0[];
extern u8 D_8007C1E8[];

extern s16 D_8019CE08;

/* One whole standard-pad diagram for the current selection: the five action
 * labels, then the five callout lines from each label to its button. */
u8 *GameDrawPadConfigDiagram(void *ot, u8 *prim) {
    prim = GameDrawPadConfigLabels(ot, prim, &D_8007C1C0[D_8019CE08 * 5]);
    return GameDrawPadConfigCallouts(
        ot, prim, &D_8007C1C0[D_8019CE08 * 5], &D_8007C1E8[D_8019CE08 * 5]);
}
