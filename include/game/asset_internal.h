#ifndef GAME_ASSET_INTERNAL_H
#define GAME_ASSET_INTERNAL_H

#include "common.h"
#include "game/menu_types.h"

#ifndef GAME_CAR_MODEL_ASSET_TYPE
#define GAME_CAR_MODEL_ASSET_TYPE void
#endif
extern GAME_CAR_MODEL_ASSET_TYPE *g_CarModelAsset;
#undef GAME_CAR_MODEL_ASSET_TYPE
extern TeamLogoSample *g_TeamLogoSampleData;
extern u8 g_TextNowLoading[];

#endif
