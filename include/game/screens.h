#ifndef GAME_SCREENS_H
#define GAME_SCREENS_H

#include "common.h"

/* Race / front-end screen drawing and text helpers, named from the format
 * strings each one references. Signatures of still-stubbed entries are
 * best-effort. */

/* Formats a lap/race time into dst as %1d'%02d"%03d, returns the write cursor. */
void *GameFormatLapTime(void *dst, s32 timeMs) asm("func_80021CD4");

void GameDrawResultScreen(void) asm("func_800200D0");         /* "RESULT" */
void GameDrawRaceEndPrompt(void) asm("func_80021654");        /* "PRESS START BUTTON" */
void GameDrawCourseIntro(void) asm("func_80021D68");          /* "COURSE IN" / "TIME ATTACK" */
void GameDrawGrandprixIntro(void) asm("func_800201D4");       /* "CLASS%d %s GRANDPRIX" */
void GameDrawEndingScreen(void) asm("func_80037D90");         /* "CONGRATULATIONS!!" */
void GameDrawTitleScreen(void) asm("func_8003479C");          /* "RAGE RACER GE" */

/*
 * Title screen and main menu. GameUpdateFrontend (func_8001BB58) runs one of the
 * four handlers below each frame via the jump table at D_8007C748, indexed by
 * the sub-state D_8009F098: 0 title -> 1 menu wipe-in -> 2 cursor/confirm ->
 * 3 fade out and request the selected scene. Details in names.md section 5.
 */

/* Enters the title screen (g_GameModeHandlers slot 3, requested when an attract
 * or real race ends); func_8001AF70 is its twin on slot 2. */
void GameEnterTitleScreen(void) asm("func_8001B014");

/* The pulsing "PRESS START" sprite: a 112x16 cell at (0x68, 0xC8), brightness
 * from func_80068568(g_AnimTimer * 96). Also drains D_801E6F28. */
void GameDrawPressStartPrompt(void) asm("func_8001B170");

/* Frontend state 0: hold on the title screen until Start is pressed. */
void GameUpdateTitleScreen(void) asm("func_8001B260");

/* Draws the five main-menu rows (112x16 cells at x = 0x68, y = 0x64 + 0x18*row);
 * the cursor row uses CLUT 0x7E86 instead of 0x7E85 and entry 1 is skipped
 * while g_AdvancedSeriesUnlocked == 0, leaving four visible rows. */
void GameDrawMainMenuRows(void) asm("func_8001B2D4");

/* Frontend state 1: the 48-frame menu wipe-in (counter D_801E6F1C to 0x30). */
void GameUpdateMainMenuOpen(void) asm("func_8001B440");

/* Frontend state 2: cursor (wrapped % 5, skipping the locked entry 1) and
 * confirm, which repoints g_CarTable / g_RaceProgress / D_8009E67C at the
 * chosen mode's records and then enters state 3. */
void GameUpdateMainMenuInput(void) asm("func_8001B5DC");

/* Frontend state 3: fades out over 0x81 frames, then requests the scene for the
 * picked row (6 / 0x1F race, 0x19 SAVE&LOAD, 0x16 OPTION). */
void GameUpdateMainMenuExit(void) asm("func_8001B884");

#endif
