#ifndef GAME_ASSET_H
#define GAME_ASSET_H

#include "common.h"

typedef struct GameAssetTripleHeader {
    s32 firstOffset;
    s32 secondOffset;
    s32 thirdOffset;
} GameAssetTripleHeader;

typedef struct GameCarModelAsset {
    u8 pad0[0x20];
    s32 modelDataOffset;
    s32 imageDataOffset;
} GameCarModelAsset;

#endif
