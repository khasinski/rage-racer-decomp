#ifndef GAME_FMV_DECODE_INTERNAL_H
#define GAME_FMV_DECODE_INTERNAL_H

#include "common.h"

extern volatile u32 *g_FmvVlcBuffers[];
#ifndef GAME_FMV_STRIP_INDEX_QUALIFIER
#define GAME_FMV_STRIP_INDEX_QUALIFIER
#endif
extern GAME_FMV_STRIP_INDEX_QUALIFIER s32 g_FmvStripIndex;
#undef GAME_FMV_STRIP_INDEX_QUALIFIER

#endif
