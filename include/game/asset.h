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
s32 LoadAsset(s32 assetIndex, void *dst) asm("func_80017C78");

/* Phase of LoadAsset's own CD state machine, 0..6 (seek, SetLoc, CdRead,
 * wait, success, failure). Sequences one transfer, unlike g_AssetLoadState. */
extern s16 g_CdLoadPhase asm("D_8007C700");

typedef struct GameCdLoadEntry {
    u32 position;
    u32 size;
} GameCdLoadEntry;

/* Disc location + size of every asset, one per g_AssetPaths[] slot, read from
 * the "\RAGE.BIN;1" index by LoadDiscArchiveIndex and rebased onto its LBA. */
extern GameCdLoadEntry g_AssetCdEntries[] asm("D_801E6834");

/* The same for the 11 streams in "\RAGE.STR;1"; BeginClassFmv picks
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

/* The offset table every asset pack starts with; sub-blocks live at
 * base + offsets[n]. Some packs only ever use the first three. */
typedef struct GameSceneAssetHeader {
    s32 offsets[11];
} GameSceneAssetHeader;

/*
 * Asset-region pointers. Spelled with a different type per translation unit
 * (u8 * / u32 / a typed header pointer), so each file carries its own aliased
 * extern rather than one declaration here; see docs/names.md 12c.
 *   g_AssetBase        D_8019C904  base of the resident block, base + base[n]
 *   g_AssetLoadCursor  D_8019CAFC  load destination, advanced by each load
 *   g_AssetSubBlockPtr D_801E8AB0  base + header->offsets[n + 1]
 *   g_ImageBlockBuffer D_801E4B30  buffer handed to func_8001A3C0
 *   g_CarModelAsset    D_8009E698  registry entry selected by SelectCarModelSlot
 *
 * g_CarModelAsset is the one slot whose pointee type really does change with
 * the screen, so its several spellings are not a naming slip to unify: the
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
void ServiceAssetLoad(void) asm("func_80019C04");
/* Cancel an in-flight load: aborts a running CdRead and clears all three
 * state words (g_CdLoadPhase / g_AssetLoadState / g_MainState). */
void ResetAssetLoader(void) asm("func_80017BE4");
/* Spin on LoadAsset until the transfer completes. */
void LoadAssetBlocking(s32 assetIndex, s32 dst) asm("func_80017E48");
/* Boot: read the "\RAGE.BIN;1" first sector into g_AssetCdEntries (135 entries)
 * and rebase the 11 "\RAGE.STR;1" stream entries. Prints "Now Searching [%s]". */
void LoadDiscArchiveIndex(void) asm("func_80017E8C");
/* LoadDiscArchiveIndex, then blocking-load asset 0 (LOGO.TMS) and upload it. */
void InitAssetSystem(void) asm("func_80018038");
/* Switch the drive to CD-DA mode (CdlSetmode 0x07 = report|autopause|CDDA);
 * the last step of every track load. */
s32 EnableCdAudioMode(void) asm("func_80017C2C");

/* Phase 1: TITLE.TMS, RG3.VH + RG3.VB (the main VAB), RES.DAT, CAR.TMS. */
s32 RequestBootAssets(void) asm("func_80018078");
void LoadBootAssets(void) asm("func_800180CC");
/* Phase 2: SAVE.TMS (memory-card screen). */
void LoadSaveScreenAssets(void) asm("func_80018344");
/* Phase 3: SELBGM.BIN, split into its SEQ / VH / VB sub-blocks. */
void LoadSelectBgmAssets(void) asm("func_80018484");
/* Phase 4: upload the SELBGM bank, load SELECT.BIN and the player's CAR_xx.1ST. */
s32 RequestCarSelectAssets(void) asm("func_80018530");
void LoadCarSelectAssets(void) asm("func_80018588");
/* Phase 5/6: one car's CAR_xx.1ST pack into the double-buffered showroom slot;
 * the "Upgraded" pair asks for modelVariant + 1, i.e. the next grade's body.
 * The *Now wrappers request and then pump ServiceAssetLoad until idle. */
void LoadCarModelNow(s32 carIndex) asm("func_80018868");
void LoadUpgradedCarModelNow(s32 carIndex) asm("func_80018A20");
/* Phase 7: OPTION.BIN. */
void LoadOptionScreenAssets(void) asm("func_80018C0C");
/* Phase 8: the GP*.TMS round screen (series * 6 + class + 0x4A) plus VOICE.BIN.
 * The request also rolls a random class when g_GrandPrixMode is 0. */
s32 RequestRoundAssets(void) asm("func_80018C88");
void LoadRoundAssets(void) asm("func_80018DF8");
/* Phase 9: the whole race load - VOICE bank, the player's CAR_xx.2ND, then the
 * course's <COURSE>n.1ST and <COURSE>n.2ND packs. */
s32 RequestRaceAssets(void) asm("func_80018FC4");
void LoadRaceAssets(void) asm("func_8001901C");
/* Phase 12: <COURSE>n.2ND, handing its 11 sub-blocks to the track subsystems. */
void LoadTrackDataAssets(void) asm("func_8001989C");
/* Unpack the already-resident <COURSE>n.1ST pack out of g_AssetBase (the same
 * work LoadRaceAssets does in its step 5). */
void InstallCourseAssets(void) asm("func_80019730");
/* Copy the live car model into g_AssetBase and re-register its bank there. */
void RelocateCarModel(void) asm("func_80018F08");

/* Asset-installation helpers. RegisterModelBank/RegisterCourseModels
 * rebase a pack's internal offsets to absolute addresses; UnrelocateModelBank
 * is the exact inverse (used before a bank is copied elsewhere). The Set*Slot
 * pair only records a pointer in a small registry that Select/Upload reads. */
void UnrelocateModelBank(void *base, s32 offset) asm("func_800179B4");
void UploadCarImage(s32 slot) asm("func_80017B5C");

#endif
