#ifndef GAME_ASSET_H
#define GAME_ASSET_H

#include "common.h"

/* Asset-load state machine phase (0 idle; 1..6 drive func_80017C78 loads). */
extern s32 g_AssetLoadState asm("D_8007BED8");

/* The asset sub-block currently being installed: `assetBase + <header offset>`,
 * then handed to func_8001A3C0 / func_8001A2E0. */
extern u8 *g_AssetBlockPtr asm("D_801F17A8");

/*
 * CD path of every loadable asset, 135 entries indexed by assetIndex:
 * [0..9] fixed assets, [10..73] the 32 car models in two halves,
 * [74..85] GP0..GP11.TMS, [86] VOICE.BIN, [87..134] the track packs
 * (index 0x57 + (course << 1) + (class << 3)).
 */
extern char *g_AssetPaths[] asm("D_8007C48C");

/* Load asset assetIndex into dst; returns loaded size/status. */
s32 GameLoadAsset(s32 assetIndex, void *dst) asm("func_80017C78");

/* Phase of GameLoadAsset's own CD state machine, 0..6 (seek, SetLoc, CdRead,
 * wait, success, failure). Sequences one transfer, unlike g_AssetLoadState. */
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

/* Disc location + size of every asset, one per g_AssetPaths[] slot, read from
 * the "\RAGE.BIN;1" index by func_80017E8C and rebased onto its LBA. */
extern GameCdLoadEntry g_AssetCdEntries[] asm("D_801E6834");

/* The same for the 11 streams in "\RAGE.STR;1"; func_80019B3C picks
 * `1 + class` in the first series and `5 + class` in the advanced one. */
extern GameCdLoadEntry g_StreamCdEntries[] asm("D_8007C6A8");

/* Boot CD scratch buffer: the RAGE.BIN index first, then asset 0. */
extern s32 g_LoadBuffer[] asm("D_8009F0B8");

typedef struct GameCarModelAsset {
    u8 pad0[0x20];
    s32 modelDataOffset;
    s32 imageDataOffset;
} GameCarModelAsset;

/* One VRAM upload record inside an image asset; func_8001A3C0 walks a chain of
 * them and func_8001A2E0 uploads each. */
typedef struct GameImageBlock {
    s32 size;   /* +0x00 block size in bytes, rounded down to a word */
    u16 x;      /* +0x04 VRAM destination */
    u16 y;      /* +0x06 */
    u16 w;      /* +0x08 in 16-bit words */
    u16 h;      /* +0x0A */
    u8 pixels[4]; /* +0x0C */
} GameImageBlock;

typedef struct GameSceneAssetHeader {
    s32 offsets[11];
} GameSceneAssetHeader;

#endif
