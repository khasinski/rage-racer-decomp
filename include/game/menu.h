#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "common.h"
#include "game/menu_types.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/vector.h"
#include "psyq/gte.h"

/*
 * Menu / UI state block at 0x8009B200 (~0x550 bytes). Retail addresses it field
 * by field as absolute globals, never through a base pointer, so the named
 * externs below - not a struct - are the usable handles.
 */

/* Grid-menu cursor (0..0x2B, a 4x11 grid; 0x2A/0x2B are the confirm buttons). */
extern s32 GameMenuCursor;
/* Non-zero while a screen transition is in progress; gates re-entry. */
extern s32 GameMenuBusy;
/* Cursor animation gate: input is only accepted while this is negative. */
extern s32 GameMenuCursorAnim;
/* Which g_MenuScreenDraw entry to run this frame, -1 for none; UpdateMenuMode
 * calls it with a step of 0x14. */
extern s32 g_MenuHandlerIndex;

/* Second, independent g_MenuScreenDraw slot, run with a step of -10; the
 * result is kept in D_8009B348. */
extern s32 g_MenuHandlerIndex2;

/* Which menu-mode screen is running; the id dispatched through
 * g_MenuScreenUpdate. Id -> screen map in docs/names.md section 3a. */
extern s32 g_MenuScreen;

/*
 * The two parallel screen tables UpdateMenuMode dispatches through, both indexed
 * by the same screen id: g_MenuScreenUpdate holds the per-frame state machines
 * (selected by g_MenuScreen) and g_MenuScreenDraw the matching fade/transition
 * overlays (selected by g_MenuHandlerIndex / g_MenuHandlerIndex2). See the
 * screen-table block at the bottom of this header for the entries.
 */
extern void (*g_MenuScreenUpdate[])(void);
extern s32 (*g_MenuScreenDraw[])(s32 step);

/*
 * Title-menu cursor, 0..4 (UpdateMainMenuInput wraps it with `(sel + 5) % 5` on the
 * up/down pad edges and skips entry 1 while g_AdvancedSeriesUnlocked is 0). 0 and 1 are the
 * two Grand Prix save files - they repoint g_CarTable / g_RaceProgress / g_CourseProgress
 * at that file's tables and set g_GrandPrixMode to 1 - 2 is Time Attack
 * (g_GrandPrixMode 0), 3 starts the attract demo and 4 opens the options.
 * DrawMainMenuRows draws the row whose index equals it as selected.
 */
extern s32 g_TitleMenuSelection;

/*
 * Element mask handed to DrawBitPatternOverlay by
 * UpdateMenuMode, selecting which parts of the current menu overlay are drawn.
 * -1 while a screen is still opening; screens then set their own pattern.
 */
extern s32 g_MenuOverlayPattern;

/* Debug/status phase code written through an asset-load state machine. */
extern s32 GameMenuLoadPhase;

/*
 * Alternate menu layout. The garage screens copy the setting into the live
 * flag on entry, RANKING / TEAM LOGO / LOGO SAMPLE force it to 0. Non-zero
 * pulls the 3D car view back (40 -> 64), shifts the HUD left by 0x2C, widens
 * the bottom bar and makes DrawScriptedSprite skip element types 9/19/29/39.
 * The setting is only ever written 0, so the layout is unreachable in retail.
 */
extern s32 g_MenuAltLayout;
extern s32 g_MenuAltLayoutSetting;

/* The two RaceRecord[series][course][5] high-score tables kept in the save block:
 * race ranking (+0x9A4) and time ranking (+0x8DC). */
extern RaceRecord g_RankingRecords[][4][5];
extern RaceRecord g_TimeRecords[][4][5];

/* The team-name entry buffer and its length, capped at 6 characters. The pair is
 * also the first bytes of the memory-card save header row. */
extern u8 g_TeamNameLength;
extern u8 g_TeamNameChars[];

/* Memory-card menu sub-state, driven by UpdateMemoryCardMenu. g_McCardStatus is the
 * last PollMemoryCardStatus result (0 no card yet, 1/2 card present,
 * -1/-2/-3 error), not a record pointer; the others are selection/phase words.
 */
extern s32 g_McMenuState;
extern s32 g_McCardStatus;
extern s32 g_McMenuSelection;
extern s32 g_McMenuPhase;
extern s32 g_McMenuSubState;

/* The two eased current/target pairs of the 3D menu view, in 1/1000 units:
 * an angle (carousel wraps at 500000 per entry) and a translation. Screens set
 * only the *Target words. */
extern s32 g_MenuViewAngle;
extern s32 g_MenuViewAngleTarget;
extern s32 g_MenuViewOffset;
extern s32 g_MenuViewOffsetTarget;

enum MenuScreenId {
    MENU_SCREEN_COURSE_SELECT = 1,
    MENU_SCREEN_RANKING,
    MENU_SCREEN_ENTER_CAR_SELECT,
    MENU_SCREEN_CAR_SELECT,
    MENU_SCREEN_CUSTOMIZE,
    MENU_SCREEN_DESIGN_MODE,
    MENU_SCREEN_TEAM_LOGO,
    MENU_SCREEN_LOGO_SAMPLE,
    MENU_SCREEN_TEAM_NAME,
    MENU_SCREEN_PAINT_COLOR,
    MENU_SCREEN_CAR_SHOP,
    MENU_SCREEN_ENGINEER_SHOP
};

enum MenuLayout {
    MENU_FADE_MAX = 0x1FC,
    MENU_FADE_COMPLETE = MENU_FADE_MAX + 1,
    MENU_FADE_INTENSITY_DIVISOR = 4,
    MENU_TEAM_NAME_MAX_LENGTH = 6,
    MENU_TEAM_NAME_GRID_COLUMNS = 11
};

/*
 * One caption of the setup-screen hint bar. `width` both sizes the sprite and
 * centres it, and `advance` is how far down the bar the next element starts.
 * Seven records, indexed by the DrawOptionHintBar argument.
 */
typedef struct OptionHintCaption {
    u8 u;
    u8 v;
    u8 width;
    u8 advance;
} OptionHintCaption;
extern OptionHintCaption g_OptionHintCaptions[];

/*
 * Slide geometry shared by the CAR SHOP and ENGINEER SHOP money panels. Both
 * count 0..25; the panel appears at 11 and rises 35 lines a frame for the next
 * eleven frames. The rise is carried in 1/32 line units because the retail
 * code multiplies then shifts rather than multiplying by 35.
 */
enum ShopPricePanelLayout {
    SHOP_PANEL_SLIDE_MAX = 25,
    SHOP_PANEL_VISIBLE_AT = 11,
    SHOP_PANEL_RISE_FRAMES = 11,
    SHOP_PANEL_RISE_PER_FRAME = 1120,
    SHOP_PANEL_RISE_SHIFT = 5,
    SHOP_PANEL_MONEY_BOX_Y = 492,
    SHOP_PANEL_MONEY_TEXT_Y = 502,
    SHOP_PANEL_PRICE_BOX_Y = 532,
    SHOP_PANEL_PRICE_TEXT_Y = 542
};

/* Slide counters for the two shop money panels. */
extern s32 g_CarShopPanelSlide;
extern s32 g_EngineerShopPanelSlide;
void DrawCarShopPricePanel(s32 step, s32 money, s32 price);
void DrawEngineerShopPricePanel(s32 step, s32 money, s32 price);

void AdjustMenuSelectionHorizontal(
    s32 *value,
    s32 min,
    s32 max);
void SetMenuBinaryChoiceVertical(s32 *value);
u16 PollMenuConfirmInput(void);
u16 PollMenuBackInput(void);
void DrawMenuFadeOverlay(s32 brightness);
void StartMenuExitFade(void);

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
    s32 caption);
void DrawMenuCursorBox(
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 flash);
void DrawFadingMenuSprites(
    s32 progress,
    s32 count,
    s32 slot);

/* Menu-mode entry: reloads the live globals from g_RaceProgress, seeds the
 * scratchpad render state, zeroes 0x8009B2F8..0x8009B378 and resets all
 * fourteen per-screen transition accumulators. */
void InitMenuMode(void);

/*
 * The menu-mode screen table pair: everything the front end shows once
 * g_MainState == 3 is one of fourteen screens, dispatched from UpdateMenuMode
 * through g_MenuScreenUpdate[g_MenuScreen] (state machine) and
 * g_MenuScreenDraw[g_MenuHandlerIndex] (fade overlay). Each Draw entry owns a
 * private accumulator in 0x8009B2C4..0x8009B2EC, clamped to [0, 0x1FC]; a
 * `step` of 0 resets it, positive fades in, negative fades out.
 * Full id -> screen table with the on-screen titles: docs/names.md section 3a.
 */

/* id 1 -- course + class picker; left/right change course, up/down the rows. */
void UpdateCourseSelectScreen(void);
s32 DrawCourseSelectScreen(s32 step);

/* id 2 -- "RANKING": total time / lap time tables, or exit back to id 1. */
void UpdateRankingScreen(void);
s32 DrawRankingScreen(s32 step);
/* The five record rows: place number + suffix + holder + row background, from
 * the ranking table g_RankingRecords or the time table g_TimeRecords. */
s32 DrawRankingTable(s32 *accumulator, s32 step, s32 table);

/* id 3 -- runs for a single frame on the way from id 1 into id 4. */
void EnterCarSelectScreen(void);

/* id 4 -- "CAR SELECT"; the hub that starts a race or opens the shops. */
void UpdateCarSelectScreen(void);
s32 DrawCarSelectScreen(s32 step);

/* id 5 -- "CUSTOMIZE": tire compound (5 settings) and transmission (AT/MT). */
void UpdateCustomizeScreen(void);
s32 DrawCustomizeScreen(s32 step);

/* id 6 -- "DESIGN MODE": livery hub, branches to team logo / name / colour. */
void UpdateDesignModeScreen(void);
s32 DrawDesignModeScreen(s32 step);

/* id 7 -- "TEAM LOGO": pick a sample logo (id 8) or hand-paint one. */
void UpdateTeamLogoScreen(void);
s32 DrawTeamLogoScreen(s32 step);

/* id 8 -- "TEAM LOGO" sample picker: character and background, 20 each. */
void UpdateLogoSampleScreen(void);
s32 DrawLogoSampleScreen(s32 step);

/*
 * id 9 -- "TEAM NAME": the 4x11 character grid driven by GameMenuCursor, with
 * cell 0x2A = BS and 0x2B = ED. Accepted characters accumulate in
 * g_TeamNameChars[g_TeamNameLength].
 */
void UpdateTeamNameScreen(void);
s32 DrawTeamNameScreen(s32 step);

/* id 10 -- "PAINT COLOR": body colour 1 and 2, 18 choices each. */
void UpdatePaintColorScreen(void);
s32 DrawPaintColorScreen(s32 step);

/* id 11 -- "SHOP" (car shop): browse every car and buy the selected one. */
void UpdateCarShopScreen(void);
s32 DrawCarShopScreen(s32 step);

/* id 12 -- "SHOP" (engineer shop): pay the tune-up fee to grade the car up. */
void UpdateEngineerShopScreen(void);
u32 DrawEngineerShopScreen(s32 step);

/*
 * Menu widgets shared across those screens. Each keeps its own accumulator and
 * follows the same `step` convention as the Draw handlers above: 0 resets and
 * draws nothing, positive ramps in, negative ramps out. docs/names.md 1 has the
 * data layouts.
 */
/* The four-bar car performance chart; only visible on CUSTOMIZE. */
/* menu_mode.c calls it with the step alone, so the parameter list stays
 * empty; the body reads (s32 step, s32 tireGrade). */
void DrawCarSpecGraph();
/* "MAX POWER <n> ps / <n> rpm" and "MAX TORQUE <n>.<n> kgm / <n> rpm". */
/* Same story as DrawCarSpecGraph: the body reads (s32 yOffset, s32 brightness)
 * and truncates them itself, while car_select.c, shop_screens.c and
 * design_screens.c all pass a third argument.  The empty list lets each
 * caller keep the argument list it was compiled with. */
void DrawCarEngineSpec();
/* The TEAM NAME 4x11 grid, its highlight and caret, and the typed name. */
void DrawTeamNameEntry(s32 step, s32 cursorIndex);
/* The 3D car view behind screens 3, 4, 5, 6, 10, 11, 12: eases
 * g_MenuViewAngle/Offset, then submits the car and the showroom floor. */
void DrawMenuCarView(void);
/* Draw and input halves of the logo painter. The canvas D_801E6F2C is a 64x64
 * 4bpp bitmap with its own 16-entry CLUT at g_TeamLogoClut. */
extern u16 g_TeamLogoClut[16];
void DrawTeamLogoCanvas(s32 panelStep, s32 editorStep);
void UpdateTeamLogoCanvas(void);

/*
 * The eight whole-canvas transforms UpdateTeamLogoCanvas offers, each
 * operating in place on g_TeamLogoCanvas (64 rows x 8 words x 8 nibbles).
 * The four scrolls wrap and play cue 1; the flips and rotations play cue 8.
 * Directions are derived from the arithmetic - see docs/names.md 13.
 */
void ScrollTeamLogoUp(void);
void ScrollTeamLogoDown(void);
void ScrollTeamLogoLeft(void);
void ScrollTeamLogoRight(void);
/* Mirror about the horizontal axis: row r <-> row 63 - r. */
void FlipTeamLogoVertical(void);
/* Mirror about the vertical axis: nibbles reversed inside each word and word
 * w swapped with word 7 - w. */
void FlipTeamLogoHorizontal(void);
/* dst(y, x) = src(x, 63 - y). func_8004BDEC is its silent second entry point. */
void RotateTeamLogoCcw(void);
/* dst(y, x) = src(63 - x, y). */
void RotateTeamLogoCw(void);

/*
 * TEAM LOGO editor data, all per-file types; see docs/names.md 12c.
 *   g_TeamLogoCanvas   D_801E6F2C  2048 bytes = 64x64 4bpp
 *   g_TeamLogoClut     g_TeamLogoClut  16 x u16
 *   g_TeamLogoRect     g_TeamLogoRect  RECT{0x290,0x30,64,16} for the canvas
 *   g_TeamLogoClutRect g_TeamLogoClutRect  RECT{16,480,16,1} for the CLUT
 * g_ClassRecords g_ClassRecords is the 11 x {s16 grade, s16 clears} table.
 */

/* Declared identically by 243 translation units before this
 * header carried them. */

extern u8 D_8007F930;
extern s32 D_8007F938;
extern s32 D_8007F940;
extern s32 D_8007F944;
extern s32 D_8007F94C;
extern s32 D_8007F954;
extern s32 D_8007F958;
extern u8 D_80082574;
extern u8 D_800825A4;
extern u8 D_80082664;
extern s32 D_8009B310;
extern s32 D_8009B324;
extern s32 D_8009B32C;
extern s32 D_8009B330;
extern s32 D_8009B33C;
extern s32 D_8009B348;
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 D_8009E6F4;
extern s32 D_8009E6F8;
extern s32 D_8009E718;
extern s32 D_8009E71C;
extern void *D_8009F0B0;
extern s32 D_8019CAB8;
extern s32 D_801E4138;
extern void *D_801E8A44;
extern s32 g_BgmChangeDelay;
extern s32 g_BgmSelectCdTrack;
extern s32 g_BgmSelectCursor;
extern s32 g_BgmSelectShowUi;
extern s32 g_BgmSelectStep;
extern s32 g_BgmSelectTrack;
extern s32 g_BgmTrackCount;
extern s32 g_BgmVolumeSetting;
extern s32 g_CarNamePlateStep;
extern s32 g_CarSwapFromIndex;
extern s32 g_CarSwapToIndex;
extern s32 g_CdTrackEnded;
extern s32 g_CourseSelectOption;
extern s32 g_CourseSwapDelay;
extern s32 g_DesignModeOption;
extern s32 g_FrontendState;
extern s32 g_MainMenuSlide;
extern u8 g_MenuBlankCaption;
extern s32 g_MenuConfirmTimer;
extern s32 g_MenuCourseModelIndex;
extern s32 g_MenuHintBarProgress;
extern s32 g_MenuHintBarStep;
extern s32 g_MenuHintButtonsVisible;
extern s32 g_MenuPendingCourseIndex;
extern s32 g_MenuPlateCarIndex;
extern u8 g_MenuSubCursor;
extern s32 g_MenuViewSpin;
extern s32 g_MonoOutput;
extern u16 g_NegconMaxTwistSaved;
extern u16 g_NegconSteerPlaySaved;
extern s16 g_NextOwnedCarIndex;
extern s32 g_OptionMenuCursor;
extern s32 g_PlayerCarY;
extern s32 g_PlayerMoney;
extern s16 g_PrevOwnedCarIndex;
extern s32 g_RankingOption;
extern s32 g_ScreenOffsetEditX;
extern s32 g_ScreenOffsetEditY;
extern s32 g_SfxVolumeSetting;
extern s32 g_SoundOptionCursor;
extern u16 g_TeamLogoSwatches[];
extern s32 g_TeamNameCharModel;
extern s32 g_TimeAttackPlateStep;
extern s32 g_TitleAttractTimer;
extern s32 g_TitleExitTimer;
extern s32 g_TitlePulse;
extern s32 g_TrackWalkStart;
extern u8 g_UiChromeScript;
extern u8 g_UiChromeScript2;

void BuildStartingGrid(void);
void ClearTeamNameTexture(void);
void DrawCarNamePlate(s32 step, s32 model, s32 grade);
void DrawMenuAltPanel(s32 stepA, s32 stepB);
void DrawMenuCourseView(void);
void DrawOptionRootMenu(void);
void DrawPadTypeHint(void);
void DrawSoundOptionScreen(void);
void DrawTimeAttackPlate(s32 stepArg);
void InitTrackLighting(void);
void InstallCarModelSlot(void);
void LoadTrackTexturePageRange(void);
s32 RequestTrackLoad(void);
void RestoreTeamLogoClut(void);
void UploadTeamLogoClut(void);

/* Declared identically by 178 translation units before this
 * header carried them. */

extern s32 D_1F800004;
extern char D_80010000[];
extern char D_80010014[];
extern char D_80010028[];
extern char D_8001004C[];
extern char D_80010078[];
extern char D_8001008C[];
extern char D_80010C30[];
extern char D_80010C40[];
extern char D_80010C44[];
extern char D_80010C50[];
extern char D_80010C5C[];
extern char D_80010C68[];
extern char D_80010C70[];
extern char D_80010D2C[];
extern const char D_80011920[4];
extern const char D_80011924[4];
extern const char D_80011928[4];
extern const char D_8001192C[8];
/* "%d": the only format string the menu code passes to LibcSprintf. */
extern u8 g_FormatDecimal[];
extern s32 D_8007C744;
extern u8 D_8007C7CC[];
extern u8 D_8007D24C;
extern s32 D_8007F948;
extern u16 D_8007F95C;
extern s32 D_8007F964;
extern s32 D_8007FB0C;
extern s32 D_8007FB10;
extern s32 D_8007FB14;
extern s32 D_8007FB18;
extern s32 D_8007FB1C;
extern s32 D_8007FB20;
extern s32 g_LogoSamplePanelSlide;
extern s32 g_TeamNameEntrySlide;
extern s32 D_8007FB2C;
extern s32 g_ClassChangeCurtainSlide;
extern s32 D_8007FB40;
extern s32 D_8007FB44;
extern s32 D_8007FB48;
extern volatile s32 D_8007FB4C;
extern u8 D_800817A0;
extern u8 D_80081818;
extern u8 D_80081890;
extern u8 D_800818CC;
extern u8 D_800819A4;
extern u8 D_80081A34;
extern u8 D_80081AD0;
extern u8 D_80081B54;
extern u8 D_80081C14;
extern u8 D_80081CA4;
extern u32 D_80081D34;
extern u8 D_80082010;
extern u8 D_800820C4;
extern u8 D_80082130;
extern s32 D_80082568;
extern u8 D_80082604;
extern u8 D_80082634;
extern u8 D_80082724;
extern u8 D_800827FC;
extern u8 D_80082814;
extern u8 D_80082844;
extern u8 D_8008285C;
extern u8 D_80082874;
extern u8 D_80082898;
extern u8 D_800828B0;
extern u8 D_800828EC;
extern u8 D_80082904;
extern u8 D_80082958;
extern u8 D_800829AC;
extern u8 D_80082A00;
extern u8 D_80082A54;
extern s32 D_80082D80[];
extern s32 D_80082E3C[];
/* The eight PS / rpm / kgm captions DrawEngineSpecLabel picks between. */
extern s32 g_EngineSpecLabels[];
extern s32 D_80082EA4;
extern s32 D_80082EA8;
extern s32 D_80082EB0;
extern s32 D_80082EB4;
extern s32 D_8009B268;
extern s32 D_8009B26C;
extern s32 D_8009B280;
extern u8 D_8009B284;
extern s32 D_8009B288;
extern s32 g_TeamNameCursorPhase;
extern s32 D_8009B290;
extern s32 D_8009B298;
extern s32 D_8009B29C;
extern u16 D_8009B2A0[16];
extern s32 D_8009B2C4;
extern s32 D_8009B2C8;
extern s32 D_8009B2CC;
extern s32 D_8009B2D0;
extern s32 D_8009B2D4;
extern s32 D_8009B2D8;
extern s32 D_8009B2DC;
extern s32 D_8009B2E0;
extern s32 D_8009B2E4;
extern s32 D_8009B2E8;
extern u32 D_8009B2EC;
extern s32 D_8009B304;
extern s32 D_8009B328;
extern s32 D_8009E6FC;
extern s32 D_8009E734;
extern s32 D_8009E7B8;
extern s16 D_8009E804;
extern s32 D_8019C770;
extern s32 D_8019C7B8;
extern s32 D_8019CDF8;
extern u16 D_8019CE9C;
extern u16 D_8019CE9E;
extern u8 D_801C0620[];
extern u16 D_801C0684;
extern u16 D_801C0686;
extern u8 D_801C0690[];
extern s32 D_801E4290;
extern s32 D_801E4294;
extern u8 D_801E4389[];
extern u8 D_801E438A[];
extern u8 D_801E438D[];
extern s32 D_801E4D74;
extern s32 D_801E6F28;
extern u8 D_801E7733[];
extern s32 D_801F17A0;
extern s32 D_801F1804;
extern char *g_BgmTrackNames[];
extern s32 g_BrowseArrowsFade;
extern s32 g_CarNamePlateFade;
extern s32 g_CarPriceTable[];
extern s32 g_CarSpecBars[4];
extern s32 g_CarSpecGraphProgress;
extern s32 g_ClassRecordMenuCursor;
extern s32 g_ExtraGrandPrixSaveMaxClass;
extern u8 g_LastValidPadType;
extern u8 g_MenuHintBarScript;
extern u8 g_NegconAxisI;
extern u8 g_NegconAxisII;
extern u8 g_NegconAxisL;
extern u8 g_NegconAxisSteer;
extern u16 g_NegconNeutralIISaved;
extern u16 g_NegconNeutralISaved;
extern u16 g_NegconNeutralLSaved;
extern s16 g_NegconPlayPercent[];
extern u16 g_NegconSteerNeutralSaved;
extern u32 g_OptionMenuExitScene;
extern s32 g_PadConfigFlipDirection;
extern s16 g_RoundScreenFadeDelays[];

void AdvanceGrandPrixClass(void);
s32 CountOwnedCars(void);
void DrawCarSlotLabel(s32 x, s32 y, s32 label);
s32 DrawClassChangeCurtain(s32 step);
void DrawClassRecordDetail(void);
void DrawClassRecordGrid(void);
void DrawOptionSceneOverlay(void);
s32 DrawPaintColorPalette(void *, s32, s32);
void DrawRaceTimePanel(s32);
void DrawScreenAdjustScreen(void);
void DrawTeamNameCharModel(void);
void DrawTireCompoundSlider(u8 x, s32 useFlag);
void DrawTitleFadeOverlay(s32 brightness);
void DrawVolumeBar(s32 level, s32 y);
void FlipCourseCard(s32 *p0, s32 *p1, s32 *p2);
/* All three return 1 while the load is still running, as defined in
 * asset/asset_requests.c and asset/car_assets.c; every caller drops it. */
s32 RequestSaveScreenAssets(void);
s32 RequestSelectBgmAssetsNoReset(void);
s32 RequestUpgradedCarModel(s32 carIndex);
void SetBodyColor1(s32);
void SetBodyColor2(s32);
void TickClassClearFanfare(void);
void UpdateCarListCursor(void);
void UpdateFrontend(void);
void UpdateOptionSceneFade(void);
s32 UpdateRoundScreenFade(s32 stage);
void UpdateTitleAttract(void);

/* Declared identically by 15 translation units before this
 * header carried them. */

extern SVec D_80011A48[];
extern Vec4 D_80011AB4;
extern Vec4 D_80011AC4;
extern Matrix D_8007C758;
extern Matrix D_8007C778;
extern DVec D_8007D5A8[];
extern Rgb D_8007D658[];
extern Matrix D_80082DFC;
extern Matrix D_80082E1C;
extern Vec4 D_8009E724;
extern Vec4 g_MenuViewScale;
extern Matrix g_SceneColorMatrix;

#endif
