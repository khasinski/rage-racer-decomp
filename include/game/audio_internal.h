#ifndef GAME_AUDIO_INTERNAL_H
#define GAME_AUDIO_INTERNAL_H

#include "common.h"

typedef struct AudioBankRuntime {
    s32 loadedMask;
    s32 cueBank;
    s32 volumeScale;
    s16 vabIds[8];
} AudioBankRuntime;

extern u8 g_ReplayFrameBuffer;
#ifndef GAME_BGM_SHUFFLE_INDEX_TYPE
#define GAME_BGM_SHUFFLE_INDEX_TYPE u32
#endif
#ifndef GAME_BGM_SHUFFLE_QUALIFIER
#define GAME_BGM_SHUFFLE_QUALIFIER
#endif
extern GAME_BGM_SHUFFLE_INDEX_TYPE g_BgmShuffleIndex;
extern GAME_BGM_SHUFFLE_QUALIFIER u8 g_BgmShuffleOrder[];
#undef GAME_BGM_SHUFFLE_INDEX_TYPE
#undef GAME_BGM_SHUFFLE_QUALIFIER

#endif
