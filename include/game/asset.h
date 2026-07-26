#ifndef GAME_ASSET_H
#define GAME_ASSET_H

#include "common.h"

/* Asset-load state machine phase (0 idle; 1..6 drive func_80017C78 loads). */
extern s32 g_AssetLoadState asm("D_8007BED8");

/*
 * The sub-block of the most recently loaded asset that is currently being
 * installed. Every writer computes it as `assetBase + <offset from the asset
 * header>` (see func_80019730 / func_80018FC4, which walk the three
 * GameAssetTripleHeader offsets) and then hands it to the installer
 * func_8001A3C0 / func_8001A2E0.
 */
extern u8 *g_AssetBlockPtr asm("D_801F17A8");

/*
 * CD path of every loadable asset, indexed by the same assetIndex that
 * GameLoadAsset takes. 135 entries:
 *   [0..9]     LOGO.TMS, TITLE.TMS, RG3.VH, RG3.VB, RES.DAT, CAR.TMS,
 *              SAVE.TMS, SELBGM.BIN, SELECT.BIN, OPTION.BIN
 *   [10..73]   CAR_xx.1ST / CAR_xx.2ND, the 32 car models in two halves
 *   [74..85]   GP0..GP11.TMS
 *   [86]       VOICE.BIN
 *   [87..134]  the track packs, 6 classes x 4 courses x 2 halves in the order
 *              BIG, MID, HI, OVAL -> index 0x57 + (course << 1) + (class << 3)
 * func_80017BD4 logs each load as "Now Loading [%s]->[0x%08x] ...".
 */
extern char *g_AssetPaths[] asm("D_8007C48C");

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
