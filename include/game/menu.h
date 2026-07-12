#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "common.h"

typedef struct GameScoreRecord {
    s16 value;
    s16 count;
} GameScoreRecord;

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

#endif
