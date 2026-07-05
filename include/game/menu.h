#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "common.h"

typedef struct GameScoreRecord {
    s16 value;
    s16 count;
} GameScoreRecord;

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
