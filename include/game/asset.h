#ifndef GAME_ASSET_H
#define GAME_ASSET_H

#include "common.h"

/* Asset-load state machine phase (0 idle; 1..6 drive func_80017C78 loads). */
extern s32 g_AssetLoadState asm("D_8007BED8");

/* Load asset assetIndex into dst; returns loaded size/status. */
s32 GameLoadAsset(s32 assetIndex, void *dst) asm("func_80017C78");

typedef struct GameAssetTripleHeader {
    s32 firstOffset;
    s32 secondOffset;
    s32 thirdOffset;
} GameAssetTripleHeader;

typedef struct GameCdLoadEntry {
    u32 position;
    u32 size;
} GameCdLoadEntry;

typedef struct GameCarModelAsset {
    u8 pad0[0x20];
    s32 modelDataOffset;
    s32 imageDataOffset;
} GameCarModelAsset;

typedef struct GameSceneAssetHeader {
    s32 offsets[11];
} GameSceneAssetHeader;

#endif
