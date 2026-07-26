#include "common.h"
#include "game/state.h"

/* Eight selectable button-mapping presets per controller, one row of eight
 * button masks each: D_8007C028 for the standard pad, D_8007C0A8 for the
 * NeGcon. */
extern u16 D_8007C028[];
extern u16 D_8007C0A8[];
/* The live mapping GameUpdatePadState reads: the pad's eight masks at +0,
 * the NeGcon's eight at +0x10. */
extern u16 D_801E4B60[];

/*
 * Installs the two selected presets into the live mapping table. Both rows are
 * copied in the same 8-iteration loop, hence the pair of source and
 * destination cursors.
 */
void GameLoadPadButtonMapping(s32 mapping0, s32 mapping1) {
    s32 i;
    u16 *dst0;
    u16 *dst1;
    u16 *src0;
    u16 *src1;
    u16 *table;

    i = 0;
    dst0 = D_801E4B60;
    dst1 = dst0 + 8;
    table = D_8007C0A8;
    src1 = table + mapping1 * 8;
    table = D_8007C028;
    src0 = table + mapping0 * 8;

    do {
        u16 mask;

        *dst0 = *src0++;
        i++;
        mask = *src1++;
        dst0++;
        *dst1++ = mask;
    } while (i < 8);
}
