#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "common.h"

typedef struct GameScoreRecord {
    s16 value;
    s16 count;
} GameScoreRecord;

/* Ranking / time high-score record row; tables D_801E7744 (ranking) and
 * D_8019CB78 (time), both [][4][5]. */
typedef struct S22 {
    s8 pad[8];   /* 0x00 name[8] */
    s32 v8;      /* 0x08 */
    s16 vC;      /* 0x0C */
    s16 vE;      /* 0x0E */
} S22;

/*
 * Menu / UI state block at 0x8009B200 (~0x550 bytes). Retail addresses it field
 * by field as absolute globals, never through a base pointer, so the named
 * externs below - not a struct - are the usable handles.
 */

/* Grid-menu cursor (0..0x2B, a 4x11 grid; 0x2A/0x2B are the confirm buttons). */
extern s32 GameMenuCursor asm("D_8009B2F4");
/* Non-zero while a screen transition is in progress; gates re-entry. */
extern s32 GameMenuBusy asm("D_8009B308");
/* Cursor animation gate: input is only accepted while this is negative. */
extern s32 GameMenuCursorAnim asm("D_8009B380");
/* Which g_MenuScreenDraw entry to run this frame, -1 for none; func_8005ACA0
 * calls it with a step of 0x14. */
extern s32 g_MenuHandlerIndex asm("D_8009B340");

/* Second, independent g_MenuScreenDraw slot, run with a step of -10; the
 * result is kept in D_8009B348. */
extern s32 g_MenuHandlerIndex2 asm("D_8009B344");

/* Which menu-mode screen is running; the id dispatched through
 * g_MenuScreenUpdate. Id -> screen map in docs/names.md section 3a. */
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
 * Element mask handed to DrawBitPatternOverlay (func_80047E60) by
 * func_8005ACA0, selecting which parts of the current menu overlay are drawn.
 * -1 while a screen is still opening; screens then set their own pattern.
 */
extern s32 g_MenuOverlayPattern asm("D_8009B318");

/* Debug/status phase code written through an asset-load state machine. */
extern s32 GameMenuLoadPhase asm("D_8009B740");

/*
 * Alternate menu layout. The garage screens copy the setting into the live
 * flag on entry, RANKING / TEAM LOGO / LOGO SAMPLE force it to 0. Non-zero
 * pulls the 3D car view back (40 -> 64), shifts the HUD left by 0x2C, widens
 * the bottom bar and makes DrawScriptedSprite skip element types 9/19/29/39.
 * The setting is only ever written 0, so the layout is unreachable in retail.
 */
extern s32 g_MenuAltLayout asm("D_8019CB0C");
extern s32 g_MenuAltLayoutSetting asm("D_8009B338");

/* The two S22[series][course][5] high-score tables kept in the save block:
 * race ranking (+0x9A4) and time ranking (+0x8DC). */
extern S22 g_RankingRecords[][4][5] asm("D_801E7744");
extern S22 g_TimeRecords[][4][5] asm("D_8019CB78");

/* The team-name entry buffer and its length, capped at 6 characters. The pair is
 * also the first bytes of the memory-card save header row. */
extern u8 g_TeamNameLength asm("D_8007F45C");
extern u8 g_TeamNameChars[] asm("D_8007F460");

/* Memory-card menu sub-state, driven by func_80061520. g_McCardStatus is the
 * last PollMemoryCardStatus result (0 no card yet, 1/2 card present,
 * -1/-2/-3 error), not a record pointer; the others are selection/phase words.
 */
extern s32 g_McMenuState asm("D_8009B71C");
extern s32 g_McCardStatus asm("D_8009B720");
extern s32 g_McMenuSelection asm("D_8009B724");
extern s32 g_McMenuPhase asm("D_8009B728");
extern s32 g_McMenuSubState asm("D_8009B72C");

/*
 * The pad word block at 0x801E4368, filled by UpdatePadState from the raw
 * BIOS buffer at D_801E403C:
 *   +0x00 status, +0x01 pad type (0x41 digital, 0x23 NeGcon),
 *   +0x02 g_PadHeld = ~(raw[0] << 8 | raw[1]), +0x04 previous frame,
 *   +0x06 g_PadEdge2 = held & ~previous, +0x08 g_PadEdge, +0x0A.. analog axes.
 * The bit order is the game's own remap, not the hardware one: 0x1000 up,
 * 0x4000 down, 0x8000 left, 0x2000 right, 0x40 cross, 0x20 circle,
 * 0x10 triangle, 0x80 square, 0x800 start - so 0x860 confirms, 0x90 cancels.
 */
extern u16 g_PadHeld asm("D_801E436A");
extern u16 g_PadEdge asm("D_801E4370");
extern u16 g_PadEdge2 asm("D_801E436E");

/* The two eased current/target pairs of the 3D menu view, in 1/1000 units:
 * an angle (carousel wraps at 500000 per entry) and a translation. Screens set
 * only the *Target words. */
extern s32 g_MenuViewAngle asm("D_8009B34C");
extern s32 g_MenuViewAngleTarget asm("D_8009B350");
extern s32 g_MenuViewOffset asm("D_8009B358");
extern s32 g_MenuViewOffsetTarget asm("D_8009B35C");

void GameAdjustMenuSelectionHorizontal(
    s32 *value,
    s32 min,
    s32 max) asm("func_800611C8");
void SetMenuBinaryChoiceVertical(s32 *value) asm("func_8006124C");
u16 PollMenuConfirmInput(void) asm("func_800612CC");
u16 PollMenuBackInput(void) asm("func_8006131C");
void DrawMenuFadeOverlay(s32 brightness) asm("func_8006136C");
void StartMenuExitFade(void) asm("func_8006138C");
void EnterMemoryCardMenu(void) asm("func_800613B8");
void EnterMemoryCardMenuFromLoad(void) asm("func_80061458");
void UpdateMemoryCardMenu(void) asm("func_80061520");

/* Menu widgets: an outlined filled box with optional caption (flags bit 0x10 =
 * caption, bit 1 = large font), the two-ring selection frame, and the timeline
 * sprites whose brightness decays through D_8009B250[] (0x1FC, -60/frame). */
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
void DrawMenuCursorBox(
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 flash) asm("func_80048D64");
void DrawFadingMenuSprites(
    s32 progress,
    s32 count,
    s32 slot) asm("func_800489AC");

/* Menu-mode entry: reloads the live globals from g_RaceProgress, seeds the
 * scratchpad render state, zeroes 0x8009B2F8..0x8009B378 and resets all
 * fourteen per-screen transition accumulators. */
void InitMenuMode(void) asm("func_80050C18");

/*
 * The menu-mode screen table pair: everything the front end shows once
 * g_MainState == 3 is one of fourteen screens, dispatched from func_8005ACA0
 * through g_MenuScreenUpdate[g_MenuScreen] (state machine) and
 * g_MenuScreenDraw[g_MenuHandlerIndex] (fade overlay). Each Draw entry owns a
 * private accumulator in 0x8009B2C4..0x8009B2EC, clamped to [0, 0x1FC]; a
 * `step` of 0 resets it, positive fades in, negative fades out.
 * Full id -> screen table with the on-screen titles: docs/names.md section 3a.
 */

/* id 1 -- course + class picker; left/right change course, up/down the rows. */
void UpdateCourseSelectScreen(void) asm("func_80053730");
s32 DrawCourseSelectScreen(s32 step) asm("func_8005290C");

/* id 2 -- "RANKING": total time / lap time tables, or exit back to id 1. */
void UpdateRankingScreen(void) asm("func_80054D10");
s32 DrawRankingScreen(s32 step) asm("func_80054C84");
/* The five record rows: place number + suffix + holder + row background, from
 * the ranking table D_801E7744 or the time table D_8019CB78. */
s32 DrawRankingTable(s32 *accumulator, s32 step, s32 table) asm("func_8004D384");

/* id 3 -- runs for a single frame on the way from id 1 into id 4. */
void EnterCarSelectScreen(void) asm("func_80055618");

/* id 4 -- "CAR SELECT"; the hub that starts a race or opens the shops. */
void UpdateCarSelectScreen(void) asm("func_8005568C");
s32 DrawCarSelectScreen(s32 step) asm("func_800551BC");

/* id 5 -- "CUSTOMIZE": tire compound (5 settings) and transmission (AT/MT). */
void UpdateCustomizeScreen(void) asm("func_800563A0");
s32 DrawCustomizeScreen(s32 step) asm("func_800562C8");

/* id 6 -- "DESIGN MODE": livery hub, branches to team logo / name / colour. */
void UpdateDesignModeScreen(void) asm("func_80057198");
s32 DrawDesignModeScreen(s32 step) asm("func_80056E64");

/* id 7 -- "TEAM LOGO": pick a sample logo (id 8) or hand-paint one. */
void UpdateTeamLogoScreen(void) asm("func_80057748");
s32 DrawTeamLogoScreen(s32 step) asm("func_800576BC");

/* id 8 -- "TEAM LOGO" sample picker: character and background, 20 each. */
void UpdateLogoSampleScreen(void) asm("func_800580C8");
s32 DrawLogoSampleScreen(s32 step) asm("func_8005803C");

/*
 * id 9 -- "TEAM NAME": the 4x11 character grid driven by GameMenuCursor, with
 * cell 0x2A = BS and 0x2B = ED. Accepted characters accumulate in
 * g_TeamNameChars[g_TeamNameLength].
 */
void UpdateTeamNameScreen(void) asm("func_8005873C");
s32 DrawTeamNameScreen(s32 step) asm("func_800586B0");

/* id 10 -- "PAINT COLOR": body colour 1 and 2, 18 choices each. */
void UpdatePaintColorScreen(void) asm("func_80058C14");
s32 DrawPaintColorScreen(s32 step) asm("func_80058B88");

/* id 11 -- "SHOP" (car shop): browse every car and buy the selected one. */
void UpdateCarShopScreen(void) asm("func_80059558");
s32 DrawCarShopScreen(s32 step) asm("func_80059248");

/* id 12 -- "SHOP" (engineer shop): pay the tune-up fee to grade the car up. */
void UpdateEngineerShopScreen(void) asm("func_8005A3A4");
u32 DrawEngineerShopScreen(s32 step) asm("func_8005A2CC");

/*
 * Menu widgets shared across those screens. Each keeps its own accumulator and
 * follows the same `step` convention as the Draw handlers above: 0 resets and
 * draws nothing, positive ramps in, negative ramps out. docs/names.md 1 has the
 * data layouts.
 */
/* The four-bar car performance chart; only visible on CUSTOMIZE. */
void DrawCarSpecGraph(s32 step, s32 tireGrade) asm("func_800496F0");
/* "MAX POWER <n> ps / <n> rpm" and "MAX TORQUE <n>.<n> kgm / <n> rpm". */
void DrawCarEngineSpec(s16 yOffset, u8 brightness, s32 unused) asm("func_80052158");
/* The TEAM NAME 4x11 grid, its highlight and caret, and the typed name. */
void DrawTeamNameEntry(s32 step, s32 cursorIndex) asm("func_8004E724");
/* The 3D car view behind screens 3, 4, 5, 6, 10, 11, 12: eases
 * g_MenuViewAngle/Offset, then submits the car and the showroom floor. */
void DrawMenuCarView(void) asm("func_8005131C");
/* Draw and input halves of the logo painter. The canvas D_801E6F2C is a 64x64
 * 4bpp bitmap with its own 16-entry CLUT at D_801E444C. */
void DrawTeamLogoCanvas(s32 panelStep, s32 editorStep) asm("func_8004A248");
void UpdateTeamLogoCanvas(void) asm("func_8004C0D8");

/*
 * The eight whole-canvas transforms UpdateTeamLogoCanvas offers, each
 * operating in place on g_TeamLogoCanvas (64 rows x 8 words x 8 nibbles).
 * The four scrolls wrap and play cue 1; the flips and rotations play cue 8.
 * Directions are derived from the arithmetic - see docs/names.md 13.
 */
void ScrollTeamLogoUp(void) asm("func_8004B9B8");
void ScrollTeamLogoDown(void) asm("func_8004BA50");
void ScrollTeamLogoLeft(void) asm("func_8004BAE4");
void ScrollTeamLogoRight(void) asm("func_8004BBA8");
/* Mirror about the horizontal axis: row r <-> row 63 - r. */
void FlipTeamLogoVertical(void) asm("func_8004BC68");
/* Mirror about the vertical axis: nibbles reversed inside each word and word
 * w swapped with word 7 - w. */
void FlipTeamLogoHorizontal(void) asm("func_8004BCE4");
/* dst(y, x) = src(x, 63 - y). func_8004BDEC is its silent second entry point. */
void RotateTeamLogoCcw(void) asm("func_8004BDB4");
/* dst(y, x) = src(63 - x, y). */
void RotateTeamLogoCw(void) asm("func_8004BF48");

/*
 * TEAM LOGO editor data, all per-file types; see docs/names.md 12c.
 *   g_TeamLogoCanvas   D_801E6F2C  2048 bytes = 64x64 4bpp
 *   g_TeamLogoClut     D_801E444C  16 x u16
 *   g_TeamLogoRect     D_8007BEE4  RECT{0x290,0x30,64,16} for the canvas
 *   g_TeamLogoClutRect D_8007BEDC  RECT{16,480,16,1} for the CLUT
 * g_ClassRecords D_8019CB40 is the 11 x {s16 grade, s16 clears} table.
 */


#endif
