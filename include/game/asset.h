#ifndef GAME_ASSET_H
#define GAME_ASSET_H

#include "common.h"
#include "psyq/gpu.h"

/* Asset-load state machine phase (0 idle; 1..6 drive LoadAsset loads). */
extern s32 g_AssetLoadState;

/* The asset sub-block currently being installed: `assetBase + <header offset>`,
 * then handed to UploadImageAsset / UploadImageBlock. */
extern u8 *g_AssetBlockPtr;

/* Its companion, the third pointer of the sub-block triple. */
extern u8 *g_AssetBlockPtr2;

/* Scratch image buffer: whatever the last screen load left free, handed to
 * LoadAsset and UploadImageAsset by the round / save / attract screens. */
extern u8 *g_ImageBlockBuffer;

/*
 * CD path of every loadable asset, 135 entries indexed by assetIndex:
 * [0..9] fixed assets, [10..73] the 32 car models in two halves,
 * [74..85] GP0..GP11.TMS, [86] VOICE.BIN, [87..134] the track packs
 * (index 0x57 + (course << 1) + (class << 3)).
 */
extern char *g_AssetPaths[];

/*
 * Index of the first entry of each variable-size family in that table. Read off
 * g_AssetPaths itself (asm/PAL/main/data/main/6BE64.data.s, resolved against the
 * string blob at D_8001008C) and cross-checked against the 135-entry RAGE.BIN
 * index on the retail PAL disc; see docs/names.md 45.
 *
 * ROUND_SCREEN: [0x4A] = "\DATA\GP0.TMS". Six screens per series, the sixth
 * being GP10 / GP11, so LoadGrandPrixScreen wants base + series * 6 + class.
 *
 * TRACK_1ST / TRACK_2ND: [0x57] = "\PACK\BIG1.1ST", [0x58] its ".2ND" sibling.
 * Four courses (BIG, MID, HI, OVAL) x two packs = eight entries per class, so
 * both are indexed base + class * 8 + course * 2. Six classes fill [0x57..0x86],
 * which is exactly the end of the table.
 */
#define ASSET_ROUND_SCREEN_BASE 0x4A
#define ASSET_TRACK_1ST_BASE    0x57
#define ASSET_TRACK_2ND_BASE    0x58

/* Load asset assetIndex into dst; returns loaded size/status. */
s32 LoadAsset(s32 assetIndex, void *dst);

/* Phase of LoadAsset's own CD state machine, 0..6 (seek, SetLoc, CdRead,
 * wait, success, failure). Sequences one transfer, unlike g_AssetLoadState. */
extern s16 g_CdLoadPhase;

typedef struct GameCdLoadEntry {
    u32 position;
    u32 size;
} GameCdLoadEntry;

/* Disc location + size of every asset, one per g_AssetPaths[] slot, read from
 * the "\RAGE.BIN;1" index by LoadDiscArchiveIndex and rebased onto its LBA. */
extern GameCdLoadEntry g_AssetCdEntries[];

/* The same for the 11 streams in "\RAGE.STR;1"; BeginClassFmv picks
 * `1 + class` in the first series and `5 + class` in the advanced one. */
extern GameCdLoadEntry g_StreamCdEntries[];

/* The one of them the streaming player is reading, set by BeginIntroFmv and
 * friends and handed straight to StartStreamRead. */
extern GameCdLoadEntry *g_StreamLoc;

/* Boot CD scratch buffer: the RAGE.BIN index first, then asset 0. */
extern s32 g_LoadBuffer[];

typedef struct GameCarModelAsset {
    u8 pad0[0x20];
    s32 modelDataOffset;
    s32 imageDataOffset;
} GameCarModelAsset;

/* One VRAM upload record inside an image asset; UploadImageAsset walks a chain of
 * them and UploadImageBlock uploads each. */
typedef struct GameImageBlock {
    s32 size;   /* +0x00 block size in bytes, rounded down to a word */
    u16 x;      /* +0x04 VRAM destination */
    u16 y;      /* +0x06 */
    u16 w;      /* +0x08 in 16-bit words */
    u16 h;      /* +0x0A */
    u8 pixels[4]; /* +0x0C */
} GameImageBlock;

/* The offset table every asset pack starts with; sub-blocks live at
 * base + offsets[n]. Some packs only ever use the first three. */
typedef struct GameSceneAssetHeader {
    s32 offsets[11];
} GameSceneAssetHeader;

/*
 * Asset-region pointers. All three address the load region in bytes: they are
 * advanced by byte counts (a load's returned size, TRACK_TEXTURE_SHADOW_SIZE,
 * g_SharedAssetWord0) and by offsets read out of the pack that happens to sit
 * there, so u8 * is the type, not the decompiler's default. A pack header is a
 * view taken of the bytes at the pointer, spelled `(GameSceneAssetHeader *)cursor`
 * where a file wants one; that is a pointer-value cast and costs nothing.
 */

/*
 * Byte size of the track-texture shadow copy StoreTeamLogoImage leaves at the
 * cursor, and hence what the cursor must skip before the .2ND pack is loaded
 * behind the .1ST. It is g_TrackTextureRect measured out: that rect is
 * {x 0x240, y 0x100, w 0x1C0, h 0x100} (asm/PAL/main/data/main/6BE64.data.s
 * 8007C710), and 0x1C0 * 0x100 16-bit pixels * 2 bytes = 0x38000 exactly.
 * Not a pack size - the largest .1ST on the disc is 0xB5830.
 */
#define TRACK_TEXTURE_SHADOW_SIZE 0x38000

/*
 * The showroom's double-buffered car-model slot. LoadCarModel /
 * LoadUpgradedCarModel load into g_CarModelBuffer or that plus one stride, so
 * the incoming model never lands on the one still being drawn; the buffer is
 * therefore two strides long, which is where g_ImageBlockBuffer starts.
 * The stride is generous rather than tight: the largest CAR_xx.1ST on the
 * retail PAL disc is 0xD4A0 (see docs/names.md 45).
 */
#define CAR_MODEL_SLOT_SIZE   0x20000
#define CAR_MODEL_BUFFER_SIZE 0x40000

/* Base of the resident asset block; sub-block n is base + base->offsets[n]. */
extern u8 *g_AssetBase;
/* Load destination, advanced past each pack as it is loaded. */
extern u8 *g_AssetLoadCursor;
/* Second sub-block cursor: base + header->offsets[n + 1]. */
extern u8 *g_AssetSubBlockPtr;

/*
 * g_CarModelAsset, by contrast, is a slot whose pointee type really does change
 * with the screen, so its several spellings are not a naming slip to unify: the
 * model viewer reads a GameCarModelAsset header, the spec screen reads an
 * EngineSpecData block and the car-select camera reads a GameRenderView.
 */

/*
 * Asset-load state machine. ServiceAssetLoad runs once per frame and
 * dispatches g_MainState 1..12 to the GameLoad*Assets step below; each step
 * advances g_AssetLoadState until it reaches 0. A screen starts a load with the
 * matching GameRequest* (which sets g_MainState and returns 1 while busy) and
 * polls the same GameRequest* until it returns 0. Asset indices are documented
 * on g_AssetPaths above; see docs/names.md 13.
 */
void ServiceAssetLoad(void);
/* Cancel an in-flight load: aborts a running CdRead and clears all three
 * state words (g_CdLoadPhase / g_AssetLoadState / g_MainState). */
void ResetAssetLoader(void);
/* Spin on LoadAsset until the transfer completes. */
void LoadAssetBlocking(s32 assetIndex, void *dst);
/* Boot: read the "\RAGE.BIN;1" first sector into g_AssetCdEntries (135 entries)
 * and rebase the 11 "\RAGE.STR;1" stream entries. Prints "Now Searching [%s]". */
void LoadDiscArchiveIndex(void);
/* LoadDiscArchiveIndex, then blocking-load asset 0 (LOGO.TMS) and upload it. */
void InitAssetSystem(void);
/* Switch the drive to CD-DA mode (CdlSetmode 0x07 = report|autopause|CDDA);
 * the last step of every track load. */
s32 EnableCdAudioMode(void);

/* Phase 1: TITLE.TMS, RG3.VH + RG3.VB (the main VAB), RES.DAT, CAR.TMS. */
s32 RequestBootAssets(void);
void LoadBootAssets(void);
/* Phase 2: SAVE.TMS (memory-card screen). */
void LoadSaveScreenAssets(void);
/* Phase 3: SELBGM.BIN, split into its SEQ / VH / VB sub-blocks. */
void LoadSelectBgmAssets(void);
/* Phase 4: upload the SELBGM bank, load SELECT.BIN and the player's CAR_xx.1ST. */
s32 RequestCarSelectAssets(void);
void LoadCarSelectAssets(void);
/* Phase 5/6: one car's CAR_xx.1ST pack into the double-buffered showroom slot;
 * the "Upgraded" pair asks for modelVariant + 1, i.e. the next grade's body.
 * The *Now wrappers request and then pump ServiceAssetLoad until idle. */
void LoadCarModelNow(s32 carIndex);
void LoadUpgradedCarModelNow(s32 carIndex);
/* Phase 7: OPTION.BIN. */
void LoadOptionScreenAssets(void);
/* Phase 8: the GP*.TMS round screen (series * 6 + class + 0x4A) plus VOICE.BIN.
 * The request also rolls a random class when g_GrandPrixMode is 0. */
s32 RequestRoundAssets(void);
void LoadRoundAssets(void);
/* Phase 9: the whole race load - VOICE bank, the player's CAR_xx.2ND, then the
 * course's <COURSE>n.1ST and <COURSE>n.2ND packs. */
s32 RequestRaceAssets(void);
void LoadRaceAssets(void);
/* Phase 12: <COURSE>n.2ND, handing its 11 sub-blocks to the track subsystems. */
void LoadTrackDataAssets(void);
/* Unpack the already-resident <COURSE>n.1ST pack out of g_AssetBase (the same
 * work LoadRaceAssets does in its step 5). */
void InstallCourseAssets(void);
/* Copy the live car model into g_AssetBase and re-register its bank there. */
void RelocateCarModel(void);

/* Asset-installation helpers. RegisterModelBank/RegisterCourseModels
 * rebase a pack's internal offsets to absolute addresses; UnrelocateModelBank
 * is the exact inverse (used before a bank is copied elsewhere). The Set*Slot
 * pair only records a pointer in a small registry that Select/Upload reads. */
void UnrelocateModelBank(s32 *base, s32 offset);
void UploadCarImage(s32 slot);

/* Declared identically by 42 translation units before this
 * header carried them. */

extern s32 g_PendingCarModelIndex;
extern u8 *g_TrackTextureShadow;

void InstallTerrainCellData(s32 *base);
void InstallTrackEventData(void *arg0);
void InstallTrackPoints(void *arg0);
void LoadCourseAssets(void);
void LoadGrandPrixScreen(void);
s32 PollAudioSlotLoad(void);
void RegisterModelBank(s32 *base, s32 index);
void RegisterCourseModels(s32 *base);
s32 RequestRaceStart(void);
void ResetTrackTextureSwap(void);
void SelectTrackCameraTable(void *arg0, s32 arg1);
/* Install sub-block 0 of the loaded .2ND track pack as the CamRow base. */
void SetTrackCameraTable(void *table);
void SetCourseObjects(void *arg0);
void SetEnvPaletteTable(void *arg0);
void SetEnvironmentScript(void *arg0);
void StoreTeamLogoImage(void *arg0);
void UploadImageAsset(void *asset);
void UploadImageBlock(void *arg0);
void UploadLoadBufferImage(void);

/* Declared identically by 19 translation units before this
 * header carried them. */

extern s32 D_8019C708;
extern s32 D_801E4144;
extern u32 g_CarImageRect;
extern void *g_CarImageSlots[];
extern void *g_CarModelSlots[];
extern void *g_ModelBanks[];
extern char g_MsgFileNotFound[];
extern char g_MsgFileReadError[];
extern char g_MsgNowLoading[];
extern char g_MsgNowSearching[];
extern char g_MsgReadBytes[];
extern char g_MsgReadSectors[];
extern char g_MsgSearchOk[];
extern char g_PathRageBin[];
extern char g_PathRageStr[];

void InstallResourceData(void *arg0);
void LoadCarModel(s32);
void LoadUpgradedCarModel(s32);
void SetCarSpec(u32 spec);

/* Declared identically by 3 translation units before this
 * header carried them. */

extern Rect g_TeamLogoClutLoadRect;
extern GpuRectPacked g_TeamLogoClutMoveRect;
extern Rect g_TrackTextureRect;

#endif
