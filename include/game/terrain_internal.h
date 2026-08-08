#ifndef GAME_TERRAIN_INTERNAL_H
#define GAME_TERRAIN_INTERNAL_H

#include "common.h"
#include "game/environment.h"

typedef union SkyUV {
    struct {
        u8 u;
        u8 v;
    } bytes;
    u16 packed;
} SkyUV;

typedef struct SkyTileUV {
    SkyUV corner[4];
} SkyTileUV;

typedef struct SkyRenderScratchpad {
    u8 *packetCursor;
    u32 *orderingTable;
    s32 cameraX;
    s32 cameraY;
    s32 cameraZ;
    s32 pad14;
    s32 pitch;
    s32 yaw;
    s32 roll;
    s32 pad24[17];
    s32 mirrorFlag;
} SkyRenderScratchpad;

typedef struct SkyClipBounds {
    s16 xMinTop;
    s16 xMaxTop;
    s16 xMinBottom;
    s16 xMaxBottom;
    s16 yEdge0;
    s16 yEdge1;
    s16 yEdge2;
    s16 yEdge3;
} SkyClipBounds;

typedef struct SkyCVec {
    u8 r;
    u8 g;
    u8 b;
    u8 cd;
} SkyCVec;

extern SkyTileUV g_SkyTileUV[];
extern GameEnvColorSlot g_EnvColors[9];

#endif
