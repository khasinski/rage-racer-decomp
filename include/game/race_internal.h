#ifndef GAME_RACE_INTERNAL_H
#define GAME_RACE_INTERNAL_H

#include "common.h"

typedef struct PrologueCameraCut {
    s16 timer;
    s16 carIndex;
} PrologueCameraCut;

typedef struct ResultPlaceBarPosition {
    u8 left;
    u8 right;
} ResultPlaceBarPosition;

typedef struct ResultPlaceSpriteLayout {
    u8 x;
    u8 y;
    u8 width;
} ResultPlaceSpriteLayout;

extern s32 g_RaceTotalTime;
#ifndef GAME_REF_SECTOR_DECL
#define GAME_REF_SECTOR_DECL extern s32 g_RefSectorTimes[3]
#endif
GAME_REF_SECTOR_DECL;
extern s32 g_PrologueStep;
extern PrologueCameraCut g_PrologueCameraCuts[];
extern ResultPlaceSpriteLayout g_ResultPlaceSprites[];
extern ResultPlaceBarPosition g_ClassPlaceBarSizes[];

#undef GAME_REF_SECTOR_DECL

#endif
