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
/*
 * Which entry of the menu overlay-handler table g_MenuScreenDraw to run this frame,
 * or -1 for none. Every menu screen sets it on entry and func_8005ACA0 does
 * `if (g_MenuHandlerIndex > 0) g_MenuScreenDraw[g_MenuHandlerIndex](0x14);`.
 */
extern s32 g_MenuHandlerIndex asm("D_8009B340");

/*
 * Second, independent slot into the same overlay-handler table g_MenuScreenDraw,
 * again -1 for none. func_8005ACA0 runs it right after g_MenuHandlerIndex with
 * a different argument and keeps the result:
 * `if (g_MenuHandlerIndex2 > 0) D_8009B348 = g_MenuScreenDraw[g_MenuHandlerIndex2](-10);`
 * GameInitMenuMode clears it to -1.
 */
extern s32 g_MenuHandlerIndex2 asm("D_8009B344");

/*
 * Which menu-mode screen is running: the id func_8005ACA0 dispatches through
 * `g_MenuScreenUpdate[g_MenuScreen]()`. The full id -> screen mapping is in the
 * screen-table block at the bottom of this header (0 bootstrap, 1 course select,
 * 2 ranking, 4 car select, 5 customize, 6 design mode, 7 team logo, 8 logo
 * sample, 9 team name, 10 paint colour, 11 car shop, 12 engineer shop).
 * GameInitMenuMode resets it to 0 and each screen writes the next id as it hands
 * over; screens 1 and 2 also switch the scratchpad render mode
 * (`(u32)(g_MenuScreen - 1) < 2`), and screen 11 draws g_CarListCursor's car
 * rather than g_PlayerCarIndex's.
 */
extern s32 g_MenuScreen asm("D_8019C9F8");

/*
 * The two parallel screen tables func_8005ACA0 dispatches through, both indexed
 * by the same screen id: g_MenuScreenUpdate holds the per-frame state machines
 * (selected by g_MenuScreen) and g_MenuScreenDraw the matching fade/transition
 * overlays (selected by g_MenuHandlerIndex / g_MenuHandlerIndex2). See the
 * screen-table block at the bottom of this header for the entries.
 */
extern void (*g_MenuScreenUpdate[])(void) asm("D_80082EB8");
extern s32 (*g_MenuScreenDraw[])(s32 step) asm("D_80082EF0");

/*
 * Title-menu cursor, 0..4 (func_8001B5DC wraps it with `(sel + 5) % 5` on the
 * up/down pad edges and skips entry 1 while g_AdvancedSeriesUnlocked is 0). 0 and 1 are the
 * two Grand Prix save files - they repoint g_CarTable / D_801E4FAC / D_8009E67C
 * at that file's tables and set g_GrandPrixMode to 1 - 2 is Time Attack
 * (g_GrandPrixMode 0), 3 starts the attract demo and 4 opens the options.
 * func_8001B2D4 draws the row whose index equals it as selected.
 */
extern s32 g_TitleMenuSelection asm("D_801E4184");

/*
 * Element mask handed to GameDrawBitPatternOverlay (func_80047E60) by
 * func_8005ACA0, selecting which parts of the current menu overlay are drawn.
 * -1 while a screen is still opening; screens then set their own pattern.
 */
extern s32 g_MenuOverlayPattern asm("D_8009B318");

/* Debug/status phase code written through an asset-load state machine. */
extern s32 GameMenuLoadPhase asm("D_8009B740");

/*
 * The team-name entry buffer and its length (previously misnamed
 * GameMenuStack / GameMenuStackDepth - it is not a screen stack). The "TEAM
 * NAME" screen (GameUpdateTeamNameScreen, id 9) appends the picked grid cell as
 * `g_TeamNameChars[g_TeamNameLength] = GameMenuCursor`, caps the length at 6,
 * writes 0xA (the blank cell) when BS pops a character, and hands the pair to
 * func_8001D530 to render the name box. The length is the first byte of the
 * memory-card save header row and the characters follow it
 * (GameWriteSaveHeaderRow / GameLoadMemoryCardSaveSlot).
 */
extern u8 g_TeamNameLength asm("D_8007F45C");
extern u8 g_TeamNameChars[] asm("D_8007F460");

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

/*
 * The pad word block at 0x801E4368, filled by func_80014014 from the raw receive
 * buffer at D_801E403C. Its layout, read straight off the stores, is
 *   +0x00 status byte, +0x01 pad type (0x41 digital, 0x23 analog),
 *   +0x02 g_PadHeld    = ~(raw[0] << 8 | raw[1])   buttons held this frame,
 *   +0x04 previous frame's g_PadHeld,
 *   +0x06 g_PadEdge2   = g_PadHeld & ~previous     buttons pressed this frame,
 *   +0x08 g_PadEdge,
 *   +0x0A.. analog axes.
 * The bit order is the game's own remap, not the hardware one (measured by
 * holding each button under an emulator): 0x1000 up, 0x4000 down, 0x8000 left,
 * 0x2000 right, 0x0040 cross, 0x0020 circle, 0x0010 triangle, 0x0080 square,
 * 0x0800 start - so 0x860 is "confirm" and 0x90 is "cancel".
 *
 * g_PadHeld is the level state, not an edge: func_8002CD4C ands it with the two
 * steering button masks, func_8001C974 tests the held 0x80C combination that
 * turns on g_MirrorMode, and func_8002317C uses `!= 0` for "any button down".
 */
extern u16 g_PadHeld asm("D_801E436A");
extern u16 g_PadEdge asm("D_801E4370");
extern u16 g_PadEdge2 asm("D_801E436E");

/*
 * The two eased scalars of the 3D menu/garage view, each a current value and the
 * target it is being driven toward, in 1/1000 units. func_80051D6C /
 * func_8005194C step the current value a fraction of the remaining distance each
 * frame and then divide by 1000 to get the value they feed to the view:
 * g_MenuViewAngle becomes the rotation angle handed to func_8001A530 (the
 * carousel wraps it at 0x7A120 = 500000 per entry), g_MenuViewOffset becomes a
 * translation component. Screens set only the *Target words; GameInitMenuMode
 * seeds g_MenuViewAngle to 500000.
 */
extern s32 g_MenuViewAngle asm("D_8009B34C");
extern s32 g_MenuViewAngleTarget asm("D_8009B350");
extern s32 g_MenuViewOffset asm("D_8009B358");
extern s32 g_MenuViewOffsetTarget asm("D_8009B35C");

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

/*
 * ---------------------------------------------------------------------------
 * The menu-mode screen table pair
 * ---------------------------------------------------------------------------
 * Everything the front end shows once g_MainState == 3 (GRAND PRIX / TIME
 * ATTACK selected on the top menu) is one of fourteen screens, dispatched from
 * func_8005ACA0 through two parallel function tables indexed by the same screen
 * id in g_MenuScreen:
 *
 *   g_MenuScreenUpdate[g_MenuScreen]()          <- per-frame state machine ("Update")
 *   g_MenuScreenDraw[g_MenuHandlerIndex](0x14)  <- fade/transition overlay ("Draw")
 *
 * Each Draw entry owns a private accumulator in the 0x8009B2C4..0x8009B2EC
 * block, clamped to [0, 0x1FC], advanced by its `step` argument and zeroed when
 * called with 0 (which is what GameInitMenuMode does to all fourteen). Slots 0,
 * 3 and 13 point at the no-op func_8005AC98, and slot 13's state machine is
 * NULL, so eleven screens are real.
 *
 * Screen ids, the on-screen title each one draws, and its accumulator (verified
 * by running the retail PAL disc under an instrumented emulator, sampling
 * g_MenuScreen every vblank and screenshotting each transition):
 *
 *   id  Update              Draw                accumulator  title / rows
 *   0   func_80052778       -                   -            menu-mode bootstrap, falls into id 1
 *   1   ..CourseSelect..    func_8005290C       (D_8009B2F0) "COURSE SELECT" / "TIME ATTACK"
 *   2   ..Ranking..         func_80054C84       D_8009B2C4   "RANKING": total time / lap time / exit
 *   3   ..EnterCarSelect..  -                   -            one-frame bridge into id 4
 *   4   ..CarSelect..       func_800551BC       D_8009B2CC   "CAR SELECT": race start / customize /
 *                                                            car shop / engineer shop / course select
 *   5   ..Customize..       func_800562C8       D_8009B2D0   "CUSTOMIZE": tire / transmission / exit
 *   6   ..DesignMode..      func_80056E64       D_8009B2D4   "DESIGN MODE": logo / name / color / exit
 *   7   ..TeamLogo..        func_800576BC       D_8009B2D8   "TEAM LOGO": sample / paint / exit
 *   8   ..LogoSample..      func_8005803C       D_8009B2DC   "TEAM LOGO": character / background / exit
 *   9   ..TeamName..        func_800586B0       D_8009B2E0   "TEAM NAME": the 4x11 character grid
 *   10  ..PaintColor..      func_80058B88       D_8009B2E4   "PAINT COLOR": body color 1 / 2 / exit
 *   11  ..CarShop..         func_80059248       D_8009B2E8   "SHOP" (car shop): buy / exit
 *   12  ..EngineerShop..    func_8005A2CC       D_8009B2EC   "SHOP" (engineer shop): tune-up / exit
 *
 * Ids 6..12 are the GRAND PRIX-only "design mode" and shop subtree; TIME ATTACK
 * only reaches 0..5. SAVE&LOAD and OPTION are *not* in this table -- they are
 * separate g_MainState scenes (2 and 7).
 *
 * Every Update runs one frame of its screen: it advances its own timed draw
 * script through GameRunTimedDrawScript, reads g_PadEdge2 (0x1000 up / 0x4000
 * down / 0x8000 left / 0x2000 right / 0x860 confirm / 0x90 cancel), and hands
 * off by writing the next screen id to g_MenuScreen and g_MenuHandlerIndex.
 *
 * Every Draw takes a signed `step`: 0 resets the accumulator, positive fades
 * the screen in, negative fades it out; the return value is the accumulator.
 */

/* id 1 -- course + class picker; left/right change course, up/down the rows. */
void GameUpdateCourseSelectScreen(void) asm("func_80053730");
s32 GameDrawCourseSelectScreen(s32 step) asm("func_8005290C");

/* id 2 -- "RANKING": total time / lap time tables, or exit back to id 1. */
void GameUpdateRankingScreen(void) asm("func_80054D10");
s32 GameDrawRankingScreen(s32 step) asm("func_80054C84");

/* id 3 -- runs for a single frame on the way from id 1 into id 4. */
void GameEnterCarSelectScreen(void) asm("func_80055618");

/* id 4 -- "CAR SELECT"; the hub that starts a race or opens the shops. */
void GameUpdateCarSelectScreen(void) asm("func_8005568C");
s32 GameDrawCarSelectScreen(s32 step) asm("func_800551BC");

/* id 5 -- "CUSTOMIZE": tire compound (5 settings) and transmission (AT/MT). */
void GameUpdateCustomizeScreen(void) asm("func_800563A0");
s32 GameDrawCustomizeScreen(s32 step) asm("func_800562C8");

/* id 6 -- "DESIGN MODE": livery hub, branches to team logo / name / colour. */
void GameUpdateDesignModeScreen(void) asm("func_80057198");
s32 GameDrawDesignModeScreen(s32 step) asm("func_80056E64");

/* id 7 -- "TEAM LOGO": pick a sample logo (id 8) or hand-paint one. */
void GameUpdateTeamLogoScreen(void) asm("func_80057748");
s32 GameDrawTeamLogoScreen(s32 step) asm("func_800576BC");

/* id 8 -- "TEAM LOGO" sample picker: character and background, 20 each. */
void GameUpdateLogoSampleScreen(void) asm("func_800580C8");
s32 GameDrawLogoSampleScreen(s32 step) asm("func_8005803C");

/*
 * id 9 -- "TEAM NAME": the 4x11 character grid driven by GameMenuCursor, with
 * cell 0x2A = BS and 0x2B = ED. Accepted characters accumulate in
 * g_TeamNameChars[g_TeamNameLength].
 */
void GameUpdateTeamNameScreen(void) asm("func_8005873C");
s32 GameDrawTeamNameScreen(s32 step) asm("func_800586B0");

/* id 10 -- "PAINT COLOR": body colour 1 and 2, 18 choices each. */
void GameUpdatePaintColorScreen(void) asm("func_80058C14");
s32 GameDrawPaintColorScreen(s32 step) asm("func_80058B88");

/* id 11 -- "SHOP" (car shop): browse every car and buy the selected one. */
void GameUpdateCarShopScreen(void) asm("func_80059558");
s32 GameDrawCarShopScreen(s32 step) asm("func_80059248");

/* id 12 -- "SHOP" (engineer shop): pay the tune-up fee to grade the car up. */
void GameUpdateEngineerShopScreen(void) asm("func_8005A3A4");
u32 GameDrawEngineerShopScreen(s32 step) asm("func_8005A2CC");

#endif
