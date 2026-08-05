#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "common.h"

/* Top-level scene/state machine, dispatched by ServiceAssetLoad; 1 = the
 * asset-load driver, other values are individual screens. */
extern s32 g_MainState asm("D_8007C704");

/* Current top-level game mode; indexes g_GameModeHandlers, dispatched each
 * frame by UpdateOptionScene. Distinct from g_MainState (the scene dispatcher). */
extern s32 g_GameMode asm("D_8019CB14");
extern void (*g_GameModeHandlers[])(void) asm("D_8007D67C");

/* MainLoop is the PS-EXE `main` and never returns: init chain, then an
 * endless per-frame loop (CD audio, sequencer, asset loads, the current
 * g_GameModeHandlers entry, VSync, display swap, UpdatePadState). */
void MainLoop(void) asm("func_80016510");
void InitSubsystems(void);

/* Controller layer. GameInitPad hands the BIOS the two 0x28-byte buffers at
 * D_801E403C / D_801E4064. UpdatePadState maintains the held / previous /
 * newly-pressed halfwords in the block at D_801E4368 (see menu.h). */
void GameInitPad(void);
void UpdatePadState(void);
void LoadPadButtonMapping(s32 mapping0, s32 mapping1);
void ApplyPadButtonMapping(void);

/* Controller-config and NeGcon calibration screens: g_GameModeHandlers entries
 * 7..11, each drawing its own screen plus the shared 3D backdrop. */
void UpdateControllerConfigScreen(void) asm("func_800155EC");
void DrawControllerConfigScreen(void);
void BeginNegconCalibration(void) asm("func_800159F8");
void UpdateNegconNeutralScreen(void) asm("func_80015AAC");
void DrawNegconNeutralScreen(void);
void UpdateNegconSteerPlayScreen(void) asm("func_80015EDC");
void DrawNegconSteerPlayScreen(void);
void UpdateNegconMaxTwistScreen(void) asm("func_80016250");
void DrawNegconMaxTwistScreen(void);
void DrawControllerSetupScene(s32 variant);

/*
 * Controller-configuration screen widgets. Two independent 0..7 selections:
 * D_8019CE08 for the standard pad, D_8019CB08 for the NeGcon (pad type byte
 * g_PadType == 0x23 picks which diagram is drawn). See docs/names.md 1.
 */
/* 16x32 arrow sprites at (0x28, 0xE0) and (0x108, 0xE0); `pulse` adds the glow. */
u8 *DrawLeftArrow(void *ot, u8 *prim, s16 x, s16 y, s32 pulse) asm("func_80014A60");
u8 *DrawRightArrow(void *ot, u8 *prim, s16 x, s16 y, s32 pulse) asm("func_80014B70");
/* Framed panel showing the selected configuration number. */
u8 *DrawPadConfigSelector(void *ot, u8 *prim, s16 x, s16 y, s32 selection) asm("func_80014C80");
/* The five action labels, and the five lines from each label to its button. */
u8 *DrawPadConfigLabels(void *ot, u8 *prim, u8 *labelRow);
u8 *DrawPadConfigCallouts(void *ot, u8 *prim, u8 *labelRow, u8 *buttonRow);
/* One whole controller diagram for the current selection: labels + callouts. */
u8 *DrawPadConfigDiagram(void *ot, u8 *prim);
u8 *DrawNegconConfigDiagram(void *ot, u8 *prim);
/* Entry hook: backs both selections up to D_8019C7A8 / D_8019C76C so a cancel
 * can restore them. Its caller sets g_GameMode = 7 in the same breath. */
void BeginControllerConfig(void) asm("func_800153FC");

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

/*
 * FMV playback ("\RAGE.STR;1" streams). One of the three GameBegin*Fmv wrappers
 * picks the stream entry in g_StreamCdEntries, records the scene to come back to
 * in g_StreamReturnScene and sets g_SceneId = 5; from then on UpdateFmv runs
 * per frame and walks D_8009F094 through 0 (start) -> 1 (decode) -> 2 (finish).
 * Start (pad bit 0x800) or the end of the stream both move it to 2.
 * The per-TU-typed members of the family - BeginFmv, StartFmvPlayback,
 * SetupFmvBuffers, InitFmvContext, OpenFmvStream,
 * PresentFmvFrame, WaitFmvDecode, StartStreamRead - keep their
 * aliased declarations in each file; see docs/names.md 13.
 */
void UpdateFmv(void) asm("func_8001E71C");
/* One decoded frame: DecDCTin the next bitstream chunk, DecDCTout the previous
 * one, then top the ring up from the drive. */
void DecodeFmvFrame(void);
/* Clear the DecDCTout callback, unhook the streamer, restore g_SceneId. */
void EndFmv(void);
/* Pull the next ready ring frame and resize the display when the stream's
 * frame size changes; returns 0 when nothing is ready. */
void *GetFmvFrame(s32 *ctx);
/* The DMA1 (MDECout) callback: LoadImage one decoded strip into VRAM and queue
 * the next strip, or flip to the other frame buffer at the end of a frame. */
void UploadFmvSlice(void);

/*
 * Boot-time defaults for everything the memory card persists: the three car
 * tables, the three GameRaceProgress slots, both course-progress blocks,
 * g_MaxClassReached, the BGM selection and the three audio settings. Called
 * once, from InitSubsystems.
 */
void InitSaveDefaults(void);
/* Reset the current g_CourseProgress block (arg < 2 also marks slot 3 free). */
void ResetCourseProgress(s32 mode);

#endif
