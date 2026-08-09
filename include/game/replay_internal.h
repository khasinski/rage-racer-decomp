#ifndef GAME_REPLAY_INTERNAL_H
#define GAME_REPLAY_INTERNAL_H

#include "common.h"
#include "game/replay.h"

#ifndef GAME_REPLAY_MODEL_TYPE
#define GAME_REPLAY_MODEL_TYPE s32
#endif

extern ReplayGrandPrixFrame *g_ReplayFramesGp;
extern ReplayTimeAttackFrame *g_ReplayFramesTimeAttack;
extern s32 g_ReplayWriteCursor;
extern s32 g_ReplayFrameCount;
extern s32 g_ReplayBufferWrapped;
extern GAME_REPLAY_MODEL_TYPE g_ReplayPlayerModel;
extern GAME_REPLAY_MODEL_TYPE g_ReplayRivalModel;

#undef GAME_REPLAY_MODEL_TYPE

#endif
