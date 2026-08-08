#ifndef GAME_REPLAY_INTERNAL_H
#define GAME_REPLAY_INTERNAL_H

#include "common.h"
#include "game/replay.h"

#ifndef GAME_REPLAY_COUNTER_TYPE
#define GAME_REPLAY_COUNTER_TYPE s32
#endif
#ifndef GAME_REPLAY_MODEL_TYPE
#define GAME_REPLAY_MODEL_TYPE s32
#endif

extern ReplayGrandPrixFrame *g_ReplayFramesGp;
extern GAME_REPLAY_COUNTER_TYPE g_ReplayWriteCursor;
extern GAME_REPLAY_COUNTER_TYPE g_ReplayFrameCount;
extern GAME_REPLAY_COUNTER_TYPE g_ReplayBufferWrapped;
extern GAME_REPLAY_MODEL_TYPE g_ReplayPlayerModel;
extern GAME_REPLAY_MODEL_TYPE g_ReplayRivalModel;

#undef GAME_REPLAY_COUNTER_TYPE
#undef GAME_REPLAY_MODEL_TYPE

#endif
