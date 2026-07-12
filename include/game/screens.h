#ifndef GAME_SCREENS_H
#define GAME_SCREENS_H

#include "common.h"

/*
 * Race / front-end screen drawing and text helpers. Names were recovered from
 * the debug/format strings each function references in the ROM (e.g. "CLASS%d
 * %s GRANDPRIX", "CONGRATULATIONS!!"), so a few signatures for still-stubbed
 * entries are best-effort until they are decompiled.
 */

/* Formats a lap/race time into dst as %1d'%02d"%03d, returns the write cursor. */
void *GameFormatLapTime(void *dst, s32 timeMs) asm("func_80021CD4");

void GameDrawResultScreen(void) asm("func_800200D0");         /* "RESULT" */
void GameDrawRaceEndPrompt(void) asm("func_80021654");        /* "PRESS START BUTTON" */
void GameDrawCourseIntro(void) asm("func_80021D68");          /* "COURSE IN" / "TIME ATTACK" */
void GameDrawGrandprixIntro(void) asm("func_800201D4");       /* "CLASS%d %s GRANDPRIX" */
void GameDrawEndingScreen(void) asm("func_80037D90");         /* "CONGRATULATIONS!!" */
void GameDrawTitleScreen(void) asm("func_8003479C");          /* "RAGE RACER GE" */

#endif
