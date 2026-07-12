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

#endif
