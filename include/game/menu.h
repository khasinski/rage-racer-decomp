#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "common.h"

typedef struct GameScoreRecord {
    s16 value;
    s16 count;
} GameScoreRecord;

/*
 * Ranking / time high-score record row. Tables D_801E7744[][4][5] (ranking) and
 * D_8019CB78[][4][5] (time). `pad[8]` holds an 8-char name.
 */
typedef struct S22 {
    s8 pad[8];   /* 0x00 name[8] */
    s32 v8;      /* 0x08 */
    s16 vC;      /* 0x0C */
    s16 vE;      /* 0x0E */
} S22;

/*
 * Global menu / UI state block at 0x8009B200 (~0x550 bytes). Accessed field by
 * field as absolute globals rather than through a base pointer, so the named
 * externs below are the usable handles; this struct documents the layout.
 * Fields are named as their semantics become clear (see func_8005873C, the grid
 * cursor handler, and func_80060524, the asset-load state machine).
 */

/* Grid-menu cursor (0..0x2B, a 4x11 grid; 0x2A/0x2B are the confirm buttons). */
extern s32 GameMenuCursor asm("D_8009B2F4");
/* Non-zero while a screen transition is in progress; gates re-entry. */
extern s32 GameMenuBusy asm("D_8009B308");
/* Cursor animation gate: input is only accepted while this is negative. */
extern s32 GameMenuCursorAnim asm("D_8009B380");
/* Debug/status phase code written through an asset-load state machine. */
extern s32 GameMenuLoadPhase asm("D_8009B740");

/* Screen-navigation stack: depth counter and the stack of screen ids. */
extern u8 GameMenuStackDepth asm("D_8007F45C");
extern u8 GameMenuStack[] asm("D_8007F460");

/*
 * Memory-card menu sub-state (part of the 0x8009B200 block), driven by the big
 * memcard-menu state machine func_80061520. g_McMenuSlotData points at the
 * currently selected save-slot record (returned by func_8005ECE0); the others
 * are selection/phase words.
 */
extern s32 g_McMenuState asm("D_8009B71C");
extern void *g_McMenuSlotData asm("D_8009B720");
extern s32 g_McMenuSelection asm("D_8009B724");
extern s32 g_McMenuPhase asm("D_8009B728");
extern s32 g_McMenuSubState asm("D_8009B72C");

/* Pad "just pressed" (edge) bits used to drive menu input. Two separate edge
 * words exist; g_PadEdge drives directional navigation, g_PadEdge2 the wider
 * button/action set. Their exact split (players vs read phase) is not settled. */
extern u16 g_PadEdge asm("D_801E4370");
extern u16 g_PadEdge2 asm("D_801E436E");

void GameAdjustMenuSelectionHorizontal(
    s32 *value,
    s32 min,
    s32 max) asm("func_800611C8");
void GameSetMenuBinaryChoiceVertical(s32 *value) asm("func_8006124C");
u16 GamePollMenuConfirmInput(void) asm("func_800612CC");
u16 GamePollMenuBackInput(void) asm("func_8006131C");
void GameDrawMenuFadeOverlay(s32 brightness) asm("func_8006136C");
void GameStartMenuExitFade(void) asm("func_8006138C");
void GameEnterMemoryCardMenu(void) asm("func_800613B8");
void GameEnterMemoryCardMenuFromLoad(void) asm("func_80061458");
void GameUpdateMemoryCardMenu(void) asm("func_80061520");

/*
 * Menu widget drawing. GameDrawMenuButton stamps an outlined, filled box with
 * an optional caption (bit 0x10 of flags = draw caption, bit 1 = large font);
 * GameDrawMenuCursorBox draws the two-ring selection frame, pulsing green off
 * the D_8009B264 phase accumulator unless `flash` selects the on/off blink.
 * GameDrawFadingMenuSprites redraws `count` timeline sprites whose per-slot
 * brightness decays through D_8009B250[] (starts at 0x1FC, -60 per frame).
 */
void GameDrawMenuButton(
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    u8 r,
    u8 g,
    u8 b,
    s32 flags,
    s32 textX,
    s32 textY,
    s32 caption) asm("func_80048B88");
void GameDrawMenuCursorBox(
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 flash) asm("func_80048D64");
void GameDrawFadingMenuSprites(
    s32 progress,
    s32 count,
    s32 slot) asm("func_800489AC");

/*
 * Menu-mode entry: reloads the view/mode words from D_801E4FAC, seeds the
 * scratchpad render state and GTE matrices, zeroes the whole 0x8009B2F8..
 * 0x8009B378 menu state block, and resets all fourteen per-screen transition
 * accumulators by calling each of them with 0.
 */
void GameInitMenuMode(void) asm("func_80050C18");

#endif
