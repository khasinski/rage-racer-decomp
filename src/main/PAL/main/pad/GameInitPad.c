#include "common.h"
#include "game/state.h"

extern u8 g_PadBuffers[] asm("D_801E403C");

void InitPad(void *buf0, s32 len0, void *buf1, s32 len1) asm("func_800631F0");
void StartPad(void) asm("func_80063200");

/* BIOS InitPAD over the two 0x28-byte pad buffers, then StartPAD. */
void GameInitPad(void) {
    InitPad(g_PadBuffers, 0x28, g_PadBuffers + 0x28, 0x28);
    StartPad();
}

/* Eight selectable button-mapping presets per controller, one row of eight
 * button masks each: g_PadButtonPresets for the standard pad, g_NegconButtonPresets for the
 * NeGcon. */
extern u16 g_PadButtonPresets[] asm("D_8007C028");
extern u16 g_NegconButtonPresets[] asm("D_8007C0A8");
/* The live mapping GameUpdatePadState reads: the pad's eight masks at +0,
 * the NeGcon's eight at +0x10. */
extern u16 g_PadButtonMapping[] asm("D_801E4B60");

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
    dst0 = g_PadButtonMapping;
    dst1 = dst0 + 8;
    table = g_NegconButtonPresets;
    src1 = table + mapping1 * 8;
    table = g_PadButtonPresets;
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

extern s16 g_PadMappingIndex asm("D_8019CE08");
extern s16 g_NegconMappingIndex asm("D_8019CB08");

/* Re-applies the button mapping from the two saved selections. */
void GameApplyPadButtonMapping(void) {
    GameLoadPadButtonMapping(g_PadMappingIndex, g_NegconMappingIndex);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/pad/GameInitPad", func_80014014);
