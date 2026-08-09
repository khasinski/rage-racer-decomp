#ifndef GAME_ASSET_INTERNAL_H
#define GAME_ASSET_INTERNAL_H

#include "common.h"
#include "game/menu_types.h"

typedef struct ModelBankHeader {
    s32 modelCount;
    s32 table;
    s32 normals;
    s32 models[1];
} ModelBankHeader;

extern void *g_CarModelAsset;
extern TeamLogoSample *g_TeamLogoSampleData;
extern u8 g_TextNowLoading[];

#endif
