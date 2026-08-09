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
extern s32 g_BgmShuffleIndex;
extern u8 g_BgmShuffleOrder[];

#endif
