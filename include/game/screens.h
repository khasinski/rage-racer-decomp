#ifndef GAME_SCREENS_H
#define GAME_SCREENS_H

#include "common.h"

/* Race / front-end screen drawing and text helpers, named from the format
 * strings each one references. Signatures of still-stubbed entries are
 * best-effort. */

/* Formats a lap/race time into dst as %1d'%02d"%03d, returns the write cursor. */
void *FormatLapTime(void *dst, s32 timeMs) asm("func_80021CD4");

void DrawResultScreen(void) asm("func_800200D0");         /* "RESULT" */
void DrawRaceEndPrompt(void) asm("func_80021654");        /* "PRESS START BUTTON" */
void DrawCourseIntro(void) asm("func_80021D68");          /* "COURSE IN" / "TIME ATTACK" */
void DrawGrandprixIntro(void) asm("func_800201D4");       /* "CLASS%d %s GRANDPRIX" */
void DrawEndingScreen(void) asm("func_80037D90");         /* "CONGRATULATIONS!!" */
/* The in-race option/pause overlay; `cursorRow` is D_801E414C. The
 * "RAGE RACER GE" string is one half of a scrolling marquee, not a title -
 * this is not the title screen. See docs/names.md 1. */
void DrawRaceOptionMenu(s32 cursorRow) asm("func_8003479C");

/*
 * Title screen and main menu. UpdateFrontend (func_8001BB58) runs one of the
 * four handlers below each frame via the jump table at D_8007C748, indexed by
 * the sub-state D_8009F098: 0 title -> 1 menu wipe-in -> 2 cursor/confirm ->
 * 3 fade out and request the selected scene. Details in names.md section 5.
 */

/* Enters the title screen (g_GameModeHandlers slot 3, requested when an attract
 * or real race ends); func_8001AF70 is its twin on slot 2. */
void EnterTitleScreen(void) asm("func_8001B014");

/* The pulsing "PRESS START" sprite: a 112x16 cell at (0x68, 0xC8), brightness
 * from func_80068568(g_AnimTimer * 96). Also drains D_801E6F28. */
void DrawPressStartPrompt(void) asm("func_8001B170");

/* Frontend state 0: hold on the title screen until Start is pressed. */
void UpdateTitleScreen(void) asm("func_8001B260");

/* Draws the five main-menu rows (112x16 cells at x = 0x68, y = 0x64 + 0x18*row);
 * the cursor row uses CLUT 0x7E86 instead of 0x7E85 and entry 1 is skipped
 * while g_AdvancedSeriesUnlocked == 0, leaving four visible rows. */
void DrawMainMenuRows(void) asm("func_8001B2D4");

/* Frontend state 1: the 48-frame menu wipe-in (counter D_801E6F1C to 0x30). */
void UpdateMainMenuOpen(void) asm("func_8001B440");

/* Frontend state 2: cursor (wrapped % 5, skipping the locked entry 1) and
 * confirm, which repoints g_CarTable / g_RaceProgress / D_8009E67C at the
 * chosen mode's records and then enters state 3. */
void UpdateMainMenuInput(void) asm("func_8001B5DC");

/* Frontend state 3: fades out over 0x81 frames, then requests the scene for the
 * picked row (6 / 0x1F race, 0x19 SAVE&LOAD, 0x16 OPTION). */
void UpdateMainMenuExit(void) asm("func_8001B884");

#endif
