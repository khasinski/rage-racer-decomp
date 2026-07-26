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

/*
 * Title screen and main menu (the func_8001Bxxx family).
 *
 * GameUpdateFrontend (func_8001BB58) is g_GameModeHandlers slot 4 and runs one
 * of the four handlers below every frame through the 4-entry jump table at
 * D_8007C748, indexed by the frontend sub-state D_8009F098:
 *
 *   0 GameUpdateTitleScreen    waits for Start (g_PadEdge2 & 0x800), then
 *                              sets D_8009F098 = 1
 *   1 GameUpdateMainMenuOpen   runs the 0x30-frame row wipe-in, then = 2
 *   2 GameUpdateMainMenuInput  cursor + confirm, then = 3
 *   3 GameUpdateMainMenuExit   fades out and requests the selected scene
 *
 * All four end by calling GameDrawMainMenuRows or GameDrawPressStartPrompt, so
 * the visible frontend is always drawn by whichever state is active.
 */

/*
 * Enters the title screen: g_GameModeHandlers slot 3, i.e. the scene the 3D
 * scene requests (func_80026570 / func_80026AE0 write g_SceneId = 3 when the
 * attract race or a race ends). Resets the frontend globals, requests scene 4
 * (GameUpdateFrontend) and draws one prompt frame. func_8001AF70 is its twin on
 * slot 2, entered from the menu/story screens (func_80022FAC, func_80053730).
 */
void GameEnterTitleScreen(void) asm("func_8001B014");

/*
 * The pulsing "PRESS START" sprite: a 112x16 cell (tpage code 0x7E84, uv
 * 0x70,0xA0) at (0x68, 0xC8), its brightness driven by
 * func_80068568(g_AnimTimer * 3 * 32) so it throbs. Also drains the
 * title-screen fade level D_801E6F28 two steps per frame.
 */
void GameDrawPressStartPrompt(void) asm("func_8001B170");

/* Frontend state 0: hold on the title screen until Start is pressed. */
void GameUpdateTitleScreen(void) asm("func_8001B260");

/*
 * Draws the five main-menu rows from the same texture page as the prompt: 112x16
 * cells at x = 0x68, y = 0x64 + 0x18*row, uv (0, 0xA0 + 0x10*entry). The row at
 * the cursor D_801E4184 gets CLUT 0x7E86 instead of 0x7E85, and entry 1 (the
 * advanced Grand Prix series) is skipped while D_8019CAC0 == 0 - which is why
 * the screen shows four rows: GRAND PRIX, TIME ATTACK, SAVE&LOAD, OPTION.
 * Each row's arg5 is the wipe-in height D_801E6F1C - 8*row clamped to 0..0x10.
 */
void GameDrawMainMenuRows(void) asm("func_8001B2D4");

/*
 * Frontend state 1: the menu opening animation. Advances the wipe-in counter
 * D_801E6F1C and hands over to state 2 the frame it reaches 0x30, which is why
 * it runs for exactly 48 frames.
 */
void GameUpdateMainMenuOpen(void) asm("func_8001B440");

/*
 * Frontend state 2: main-menu cursor and confirm. Up (0x1000) / Down (0x4000)
 * step D_801E4184, which is wrapped with % 5 and skips the locked entry 1;
 * every actual move plays cue 1. On confirm (0x860) it plays cue 2, reseeds the
 * opponent order (func_8001B488), points the save-record pointers at the record
 * set for the chosen mode and either requests a track load or the sub-screen
 * (func_800182D0 = SAVE&LOAD, func_80018B98 = OPTION), then enters state 3.
 */
void GameUpdateMainMenuInput(void) asm("func_8001B5DC");

/*
 * Frontend state 3: fades out over 0x81 frames (func_80033AA0(2*t, 0x59)) and
 * then requests the scene for the row that was picked - 6 / 0x1F for the two
 * Grand Prix entries and Time Attack (with D_801E4DAC = 1 for Grand Prix, 0 for
 * Time Attack), 0x19 for SAVE&LOAD, 0x16 for OPTION.
 */
void GameUpdateMainMenuExit(void) asm("func_8001B884");

#endif
