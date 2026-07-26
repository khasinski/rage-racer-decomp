#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "common.h"

/*
 * Top-level game scene/state machine, dispatched by func_80019C04: each value
 * selects a scene handler (1 = asset-load driver func_800180CC, others are the
 * individual screens). See also g_AssetLoadState in asset.h.
 */
extern s32 g_MainState asm("D_8007C704");

/* Current top-level game mode; indexes g_GameModeHandlers, dispatched each
 * frame by func_80025870. Distinct from g_MainState (the scene dispatcher). */
extern s32 g_GameMode asm("D_8019CB14");
extern void (*g_GameModeHandlers[])(void) asm("D_8007D67C");

/*
 * Boot and frame spine (the first 0x3900 bytes of .text, rom 0x4748).
 *
 * GameMainLoop is the PS-EXE `main`: the crt0 entry stub at 0x800630B4 calls
 * it, it runs the one-shot init chain (GameInitSubsystems, func_80018038, ...)
 * and then never returns - it falls into an endless per-frame loop that ticks
 * CD audio, the sequencer and the pending asset loads, dispatches the current
 * screen through g_GameModeHandlers (indexed by D_801E42E4), waits on VSync,
 * swaps the display env and finally calls GameUpdatePadState. The frontend
 * (func_8001BB58) and the 3D scene (func_80026AE0) are two entries of that
 * table, which is why they are mutually exclusive per frame.
 */
void GameMainLoop(void) asm("func_80016510");
void GameInitSubsystems(void) asm("func_800163C4");

/*
 * Controller layer. GameInitPad calls the BIOS InitPAD with the two 0x28-byte
 * buffers at D_801E403C / D_801E4064 and then StartPAD.
 * GameUpdatePadState runs once per frame at the bottom of GameMainLoop: it
 * copies the raw buffers, recognises pad type 0x41 (digital) and 0x23
 * (NeGcon), inverts the active-low button words and maintains
 * current / previous / newly-pressed halfwords in the block at D_801E4368.
 * GameLoadPadButtonMapping copies one 8-entry row of button bitmasks out of
 * D_8007C0A8 and one out of D_8007C128 into D_801E4B60 / D_801E4B70;
 * GameApplyPadButtonMapping re-applies it from the two saved selections.
 */
void GameInitPad(void) asm("func_80013F48");
void GameUpdatePadState(void) asm("func_80014014");
void GameLoadPadButtonMapping(s32 mapping0, s32 mapping1) asm("func_80013F80");
void GameApplyPadButtonMapping(void) asm("func_80013FE4");

/*
 * Controller-configuration and NeGcon calibration screens: g_GameModeHandlers
 * entries 7..11, each an update handler that draws its own screen and then the
 * shared 3D backdrop (GameDrawControllerSetupScene). Identified by the strings
 * they draw: "INSERT CONTROLLER" / "CONTROLLER ERROR" (config screen),
 * "Hold the \"NeGcon\" in an untwisted position and press start button.",
 * "Steer play." and "Maximum twist.".
 */
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

#endif
