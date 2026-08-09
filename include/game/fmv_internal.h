#ifndef GAME_FMV_INTERNAL_H
#define GAME_FMV_INTERNAL_H

#include "common.h"
#include "psyq/gpu.h"

#ifndef GAME_FMV_STATE_QUALIFIER
#define GAME_FMV_STATE_QUALIFIER
#endif

typedef union FmvStripRectTable {
    Rect rects[2];
    volatile s16 components[2][4];
} FmvStripRectTable;

extern FmvStripRectTable g_FmvStripRects;
extern volatile u32 *g_FmvVlcBuffer0;
extern volatile u32 *g_FmvVlcBuffer1;
extern volatile u32 *g_FmvStripBuffer0;
extern volatile u32 *g_FmvStripBuffer1;
extern GAME_FMV_STATE_QUALIFIER u16 g_DispEnv0Y;
extern GAME_FMV_STATE_QUALIFIER u16 g_DispEnv1X;
extern GAME_FMV_STATE_QUALIFIER u16 g_DispEnv1Y;
extern GAME_FMV_STATE_QUALIFIER s32 g_FrameParity;
extern s32 g_StreamReturnScene;

#undef GAME_FMV_STATE_QUALIFIER
#endif
