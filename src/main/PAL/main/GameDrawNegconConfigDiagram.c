#include "common.h"
#include "game/state.h"

/* The NeGcon counterparts of D_8007C1C0 / D_8007C1E8. */
extern u8 D_8007C210[];
extern u8 D_8007C238[];

extern s16 D_8019CB08;

/* One whole NeGcon diagram for the current selection: labels, then callouts. */
u8 *GameDrawNegconConfigDiagram(void *ot, u8 *prim) {
    prim = GameDrawPadConfigLabels(ot, prim, &D_8007C210[D_8019CB08 * 5]);
    return GameDrawPadConfigCallouts(
        ot, prim, &D_8007C210[D_8019CB08 * 5], &D_8007C238[D_8019CB08 * 5]);
}
