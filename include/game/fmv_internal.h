#ifndef GAME_FMV_INTERNAL_H
#define GAME_FMV_INTERNAL_H

#include "common.h"
#include "psyq/gpu.h"

#ifndef GAME_FMV_STATE_QUALIFIER
#define GAME_FMV_STATE_QUALIFIER
#endif

extern Rect g_FmvStripRects[2];
extern volatile u32 *g_FmvVlcBuffer0;
extern volatile u32 *g_FmvVlcBuffer1;
extern volatile u32 *g_FmvStripBuffer0;
extern volatile u32 *g_FmvStripBuffer1;
extern GAME_FMV_STATE_QUALIFIER u16 g_DispEnv0Y;
extern GAME_FMV_STATE_QUALIFIER u16 g_DispEnv1X;
extern GAME_FMV_STATE_QUALIFIER u16 g_DispEnv1Y;
extern GAME_FMV_STATE_QUALIFIER s32 g_FrameParity;
#ifndef GAME_STREAM_RETURN_TYPE
#define GAME_STREAM_RETURN_TYPE s32
#endif
extern GAME_STREAM_RETURN_TYPE g_StreamReturnScene;

#undef GAME_FMV_STATE_QUALIFIER
#undef GAME_STREAM_RETURN_TYPE
#endif
