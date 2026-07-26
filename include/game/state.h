#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "common.h"

/* Top-level scene/state machine, dispatched by func_80019C04; 1 = the
 * asset-load driver, other values are individual screens. */
extern s32 g_MainState asm("D_8007C704");

/* Current top-level game mode; indexes g_GameModeHandlers, dispatched each
 * frame by func_80025870. Distinct from g_MainState (the scene dispatcher). */
extern s32 g_GameMode asm("D_8019CB14");
extern void (*g_GameModeHandlers[])(void) asm("D_8007D67C");

/* GameMainLoop is the PS-EXE `main` and never returns: init chain, then an
 * endless per-frame loop (CD audio, sequencer, asset loads, the current
 * g_GameModeHandlers entry, VSync, display swap, GameUpdatePadState). */
void GameMainLoop(void) asm("func_80016510");
void GameInitSubsystems(void) asm("func_800163C4");

/* Controller layer. GameInitPad hands the BIOS the two 0x28-byte buffers at
 * D_801E403C / D_801E4064. GameUpdatePadState maintains the held / previous /
 * newly-pressed halfwords in the block at D_801E4368 (see menu.h). */
void GameInitPad(void) asm("func_80013F48");
void GameUpdatePadState(void) asm("func_80014014");
void GameLoadPadButtonMapping(s32 mapping0, s32 mapping1) asm("func_80013F80");
void GameApplyPadButtonMapping(void) asm("func_80013FE4");

/* Controller-config and NeGcon calibration screens: g_GameModeHandlers entries
 * 7..11, each drawing its own screen plus the shared 3D backdrop. */
void GameUpdateControllerConfigScreen(void) asm("func_800155EC");
void GameDrawControllerConfigScreen(void) asm("func_80015444");
void GameBeginNegconCalibration(void) asm("func_800159F8");
void GameUpdateNegconNeutralScreen(void) asm("func_80015AAC");
void GameDrawNegconNeutralScreen(void) asm("func_80015928");
void GameUpdateNegconSteerPlayScreen(void) asm("func_80015EDC");
void GameDrawNegconSteerPlayScreen(void) asm("func_80015B78");
void GameUpdateNegconMaxTwistScreen(void) asm("func_80016250");
void GameDrawNegconMaxTwistScreen(void) asm("func_80016064");
void GameDrawControllerSetupScene(s32 variant) asm("func_80014618");

/*
 * Controller-configuration screen widgets. Two independent 0..7 selections:
 * D_8019CE08 for the standard pad, D_8019CB08 for the NeGcon (pad type byte
 * g_PadType == 0x23 picks which diagram is drawn). See docs/names.md 1.
 */
/* 16x32 arrow sprites at (0x28, 0xE0) and (0x108, 0xE0); `pulse` adds the glow. */
u8 *GameDrawLeftArrow(void *ot, u8 *prim, s16 x, s16 y, s32 pulse) asm("func_80014A60");
u8 *GameDrawRightArrow(void *ot, u8 *prim, s16 x, s16 y, s32 pulse) asm("func_80014B70");
/* Framed panel showing the selected configuration number. */
u8 *GameDrawPadConfigSelector(void *ot, u8 *prim, s16 x, s16 y, s32 selection) asm("func_80014C80");
/* The five action labels, and the five lines from each label to its button. */
u8 *GameDrawPadConfigLabels(void *ot, u8 *prim, u8 *labelRow) asm("func_80014EAC");
u8 *GameDrawPadConfigCallouts(void *ot, u8 *prim, u8 *labelRow, u8 *buttonRow) asm("func_800151B0");
/* One whole controller diagram for the current selection: labels + callouts. */
u8 *GameDrawPadConfigDiagram(void *ot, u8 *prim) asm("func_8001530C");
u8 *GameDrawNegconConfigDiagram(void *ot, u8 *prim) asm("func_80015384");
/* Entry hook: backs both selections up to D_8019C7A8 / D_8019C76C so a cancel
 * can restore them. Its caller sets g_GameMode = 7 in the same breath. */
void GameBeginControllerConfig(void) asm("func_800153FC");

/* Identity of the running scene: queried (`== 0xC`, `== 0x11`, `== 0x1E`, ...)
 * but never dispatched. Every writer also resets g_SceneTimer. */
extern s32 g_SceneId asm("D_801E42E4");

/* Per-scene frame counter, reset with every g_SceneId write. Scenes sequence
 * themselves against fixed thresholds. Four TUs need it as u32 and carry their
 * own unsigned declaration of the same symbol. */
extern s32 g_SceneTimer asm("D_801E40B8");

/* Free-running animation phase counter: drives cyclic effects (sine offsets,
 * blink tests `& 2` / `& 8`, `% 6` cycles), never a deadline. */
extern s32 g_AnimTimer asm("D_8009E694");

#endif
