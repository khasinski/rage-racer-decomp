#ifndef GAME_WORK_BUFFER_H
#define GAME_WORK_BUFFER_H

#include "common.h"
#include "game/replay.h"

typedef struct SpuTransferSampleBuffer {
    u8 transferArea[0x800];
    s16 channelA[2][0x100];
    s16 channelB[2][0x100];
} SpuTransferSampleBuffer;

typedef union GameWorkBuffer {
    u8 bytes[0x2EE0];
    SpuTransferSampleBuffer spuTransfer;
    ReplayGrandPrixFrame grandPrixReplay[1];
    ReplayTimeAttackFrame timeAttackReplay[1];
} GameWorkBuffer;

extern GameWorkBuffer g_ReplayFrameBuffer;

#endif
