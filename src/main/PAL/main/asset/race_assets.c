#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"
#include "game/cd.h"
#include "game/menu.h"

/*
 * Every `X + *(s32 *)(X + 4*k)` below is sub-block k of the loaded asset
 * pack's GameSceneAssetHeader offset table (game/asset.h).
 */
#define ASSET_SUB(base, k) ((base) + ((GameSceneAssetHeader *)(base))->offsets[k])

/* Asset-region pointers, each spelled with this file's own view of the
 * pointee; game/asset.h explains why they are not one shared declaration. */
extern u8 *g_AssetLoadCursor;
extern u8 *g_AssetSubBlockPtr;
extern u8 *g_AssetBlockPtr2;
void StartAudioSlotLoad(s32 slot, void *header, void *body, void *table);
void UploadImageAsset(void *asset);
void SetTrackCameraTable(void *table);
void RegisterCourseModels(void *base);
extern s32 g_ImageBlockBuffer;
extern s32 g_AssetBase;

s32 RequestRaceAssets(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 9) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 9;
    g_AssetLoadState = 1;
    return 1;
}

void LoadRaceAssets(void) {
    switch (g_AssetLoadState) {
    case 1: {
        s32 *src = (s32 *)g_AssetBlockPtr;
        s32 size = g_SharedAssetWord0;
        s32 *dst = (s32 *)g_AssetLoadCursor;
        s32 n = size / 4;
        while (n != 0) {
            *dst = *src;
            src++;
            n--;
            dst++;
        }
        StartAudioSlotLoad(2, g_AssetLoadCursor, g_AssetSubBlockPtr, 0);
        g_AssetLoadState = 2;
        g_AssetLoadCursor = g_AssetLoadCursor + g_SharedAssetWord0;
        break;
    }
    case 2:
        if ((s16)PollAudioSlotLoad() != 0) {
            g_AssetLoadState = 3;
        }
        break;
    case 3: {
        s32 carIndex = g_PlayerCarIndex;
        s32 carAsset = GetCarAssetIndex(carIndex, g_CarTable[carIndex].modelVariant);
        if (LoadAsset((carAsset * 2) + 11, g_AssetLoadCursor) != 0) {
            GameSceneAssetHeader *pack;
            s32 offset;
            u8 *table;
            u8 *header;
            u8 *body;
            pack = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = pack->offsets[0];
            g_AssetBlockPtr = (u8 *)pack + offset;
            SetCarSpec((u32)g_AssetBlockPtr);
            table = g_AssetLoadCursor;
            header = ASSET_SUB(table, 1);
            body = ASSET_SUB(table, 3);
            table = ASSET_SUB(table, 2);
            g_AssetBlockPtr = header;
            g_AssetBlockPtr2 = table;
            g_AssetSubBlockPtr = body;
            StartAudioSlotLoad(3, header, body, table);
            pack = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = pack->offsets[4];
            g_AssetBlockPtr = (u8 *)pack + offset;
            UploadImageAsset(g_AssetBlockPtr);
            g_AssetLoadState = 4;
            g_AssetLoadCursor = g_AssetSubBlockPtr;
        }
        break;
    }
    case 4:
        if ((s16)PollAudioSlotLoad() != 0) {
            g_AssetLoadState = 5;
        }
        break;
    case 5: {
        u8 *dst;
        s32 courseOffset;
        s32 classBase;
        dst = g_AssetLoadCursor;
        courseOffset = g_CourseIndex * 2;
        classBase = (g_GrandPrixClass * 8) + 0x57;
        if (LoadAsset(courseOffset + classBase, dst) != 0) {
            GameSceneAssetHeader *pack;
            s32 offset;
            u8 *base;
            s32 logoOffset, shadowOffset;
            pack = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = pack->offsets[0];
            g_AssetBlockPtr = (u8 *)pack + offset;
            UploadImageAsset(g_AssetBlockPtr);
            pack = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = pack->offsets[1];
            g_AssetBlockPtr = (u8 *)pack + offset;
            UploadImageAsset(g_AssetBlockPtr);
            pack = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = pack->offsets[2];
            g_AssetBlockPtr = (u8 *)pack + offset;
            UploadImageBlock(g_AssetBlockPtr);
            base = g_AssetLoadCursor;
            logoOffset = ((GameSceneAssetHeader *)base)->offsets[3];
            shadowOffset = ((GameSceneAssetHeader *)base)->offsets[4];
            g_AssetBlockPtr = base + logoOffset;
            g_AssetSubBlockPtr = base + shadowOffset;
            UploadImageAsset(g_AssetBlockPtr);
            StoreTeamLogoImage(g_AssetLoadCursor);
            g_TrackTextureShadow = g_AssetLoadCursor;
            UploadImageAsset(g_AssetSubBlockPtr);
            ResetTrackTextureSwap();
            g_AssetLoadState = 6;
            g_AssetLoadCursor = g_AssetLoadCursor + 0x38000;
        }
        break;
    }
    case 6: {
        u8 *dst;
        s32 courseOffset;
        /* The last crutch in this directory. Retail computes this sum in v0
         * and adds the 0x58 into a0; gcc 2.6.3 ties the sum straight into a0,
         * which is the same instructions in different registers, 16 of them.
         * Ten source shapes give the identical 16 (declaration order, dst
         * hoisted late, the constant folded into either term or added last,
         * the two shifts split into their own locals, << instead of *,
         * operands reversed, the globals read inline) - it is a register
         * allocation fixpoint, not a spelling. */
        register s32 assetIndex asm("$2");
        dst = g_AssetLoadCursor;
        courseOffset = g_CourseIndex * 2;
        assetIndex = (g_GrandPrixClass * 8) + courseOffset;
        if (LoadAsset(assetIndex + 0x58, dst) != 0) {
            GameSceneAssetHeader *header;
            s32 offset;

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[0];
            g_AssetBlockPtr = (u8 *)header + offset;
            SetTrackCameraTable(g_AssetBlockPtr);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[1];
            g_AssetBlockPtr = (u8 *)header + offset;
            SetEnvPaletteTable(g_AssetBlockPtr);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[2];
            g_AssetBlockPtr = (u8 *)header + offset;
            SetEnvironmentScript(g_AssetBlockPtr);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[3];
            g_AssetBlockPtr = (u8 *)header + offset;
            RegisterModelBank(g_AssetBlockPtr, 1);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[4];
            g_AssetBlockPtr = (u8 *)header + offset;
            InstallTrackPoints(g_AssetBlockPtr);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[5];
            g_AssetBlockPtr = (u8 *)header + offset;
            RegisterCourseModels(g_AssetBlockPtr);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[6];
            g_AssetBlockPtr = (u8 *)header + offset;
            RegisterModelBank(g_AssetBlockPtr, 2);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[7];
            g_AssetBlockPtr = (u8 *)header + offset;
            InstallTerrainCellData(g_AssetBlockPtr);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[8];
            g_AssetBlockPtr = (u8 *)header + offset;
            SetCourseObjects(g_AssetBlockPtr);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[9];
            g_AssetBlockPtr = (u8 *)header + offset;
            InstallTrackEventData(g_AssetBlockPtr);

            header = (GameSceneAssetHeader *)g_AssetLoadCursor;
            offset = header->offsets[10];
            g_AssetBlockPtr = (u8 *)header + offset;
            SelectTrackCameraTable(g_AssetBlockPtr, 1);

            g_AssetLoadState = 7;
        }
        break;
    }
    case 7:
        if (EnableCdAudioMode() != 0) {
            g_AssetLoadState = 0;
        }
        break;
    }
}

s32 RequestRaceStart(void) {
    s32 state;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    state = 10;
    if (g_MainState == state) {
        g_MainState = 0;
        return 0;
    }

    ResetCdAudioState();
    g_MainState = state;
    g_AssetLoadState = 1;
    return 1;
}

void LoadGrandPrixScreen(void) {
    s32 base;
    s32 offset;
    s32 loaded;

    if (g_AssetLoadState == 1) {
        offset = g_GrandPrixSeries * 6;
        base = g_GrandPrixClass + 0x4A;
        loaded = LoadAsset((s32)(offset + base), (void *)g_ImageBlockBuffer);
        if (loaded != 0) {
            g_AssetLoadState = 0;
        }
    }
}

s32 RequestTrackLoad(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 0xB) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 0xB;
    g_AssetLoadState = 1;
    return 1;
}

void LoadCourseAssets(void) {
    s32 loaded;

    if (g_AssetLoadState == 1) {
        s32 courseOffset = g_CourseIndex * 2;
        s32 classBase = (g_GrandPrixClass * 8) + 0x57;

        loaded = LoadAsset((s32)(courseOffset + classBase), (void *)g_AssetBase);
        if (loaded != 0) {
            g_AssetLoadState = 0;
            g_ImageBlockBuffer = loaded + g_AssetBase;
        }
    }
}
