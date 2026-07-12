#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "common.h"

/*
 * Top-level game scene/state machine, dispatched by func_80019C04: each value
 * selects a scene handler (1 = asset-load driver func_800180CC, others are the
 * individual screens). See also g_AssetLoadState in asset.h.
 */
extern s32 g_MainState asm("D_8007C704");

#endif
