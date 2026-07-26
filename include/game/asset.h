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

/*
 * Phase of GameLoadAsset's own CD state machine, 0..6 (0 idle / start the seek,
 * 1..2 SetLoc + wait, 3 CdRead, 4 wait for the read, 5 success -> log "read:%d",
 * 6 failure -> log "File read error:%s"). Distinct from g_AssetLoadState, which
 * sequences whole assets; this one sequences a single CD transfer.
 * func_80017BE4 forces it back to 0 when a load is abandoned.
 */
extern s16 g_CdLoadPhase asm("D_8007C700");

typedef struct GameAssetTripleHeader {
    s32 firstOffset;
    s32 secondOffset;
    s32 thirdOffset;
} GameAssetTripleHeader;

typedef struct GameCdLoadEntry {
    u32 position;
    u32 size;
} GameCdLoadEntry;

/*
 * Disc location + size of every asset, one entry per g_AssetPaths[] slot (135).
 * func_80017E8C reads the "\RAGE.BIN;1" index into g_LoadBuffer as (offset,size)
 * pairs and rebases each offset onto RAGE.BIN's own LBA
 * (`CdIntToPos(base + offset, &entry)`); GameLoadAsset then seeks straight to
 * g_AssetCdEntries[assetIndex].
 */
extern GameCdLoadEntry g_AssetCdEntries[] asm("D_801E6834");

/*
 * The same thing for the 11 streamed CD-XA tracks inside "\RAGE.STR;1", rebased
 * by the second half of func_80017E8C. func_80019B3C picks the track for the
 * current class as `1 + g_GrandPrixClass` in the first series and
 * `5 + g_GrandPrixClass` in the advanced one.
 */
extern GameCdLoadEntry g_StreamCdEntries[] asm("D_8007C6A8");

/*
 * Boot-time CD scratch buffer at 0x8009F0B8: func_80017E8C reads the RAGE.BIN
 * index into it, then func_80018038 reuses it as the destination of asset 0.
 */
extern s32 g_LoadBuffer[] asm("D_8009F0B8");

typedef struct GameCarModelAsset {
    u8 pad0[0x20];
    s32 modelDataOffset;
    s32 imageDataOffset;
} GameCarModelAsset;

typedef struct GameSceneAssetHeader {
    s32 offsets[11];
} GameSceneAssetHeader;

#endif
