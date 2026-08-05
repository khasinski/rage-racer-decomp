#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"
#include "game/cd.h"
#include "game/menu.h"
s32 LoadAsset(s32 arg0, void *arg1);

/*
 * Every `X + *(s32 *)(X + 4*k)` below is sub-block k of the loaded asset
 * pack's GameSceneAssetHeader offset table (game/asset.h).
 */
#define ASSET_SUB(base, k) ((base) + ((GameSceneAssetHeader *)(base))->offsets[k])

extern u8 *g_AssetLoadCursor;
extern u8 *g_AssetSubBlockPtr;
extern u8 *g_AssetBlockPtr2;
void StartAudioSlotLoad(s32 arg0, void *arg1, void *arg2, void *arg3);
s32 GetCarAssetIndex(s32 model, s32 grade);
void UploadImageAsset(void *arg0);
void SetTrackCameraTable(void *arg0);
void RegisterCourseModels(void *arg0);
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
        s32 raw = g_SharedAssetWord0;
        s32 *dst = (s32 *)g_AssetLoadCursor;
        s32 n = raw / 4;
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
        if ((PollAudioSlotLoad() << 16) != 0) {
            g_AssetLoadState = 3;
        }
        break;
    case 3: {
        s32 idx = g_PlayerCarIndex;
        s32 sz = GetCarAssetIndex(idx, g_CarTable[idx].modelVariant);
        if (LoadAsset((sz * 2) + 11, g_AssetLoadCursor) != 0) {
            register u8 *base_a0 asm("$4");
            u8 *base_a3;
            u8 *p1;
            u8 *p2;
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 0);
            SetCarSpec();
            base_a3 = g_AssetLoadCursor;
            p1 = ASSET_SUB(base_a3, 1);
            p2 = ASSET_SUB(base_a3, 3);
            base_a3 = ASSET_SUB(base_a3, 2);
            g_AssetBlockPtr = p1;
            g_AssetBlockPtr2 = base_a3;
            g_AssetSubBlockPtr = p2;
            StartAudioSlotLoad(3, p1, p2, base_a3);
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 4);
            UploadImageAsset(g_AssetBlockPtr);
            g_AssetLoadState = 4;
            g_AssetLoadCursor = g_AssetSubBlockPtr;
        }
        break;
    }
    case 4:
        if ((PollAudioSlotLoad() << 16) != 0) {
            g_AssetLoadState = 5;
        }
        break;
    case 5: {
        u8 *p;
        s32 scaled;
        s32 base_off;
        p = g_AssetLoadCursor;
        scaled = g_CourseIndex * 2;
        base_off = (g_GrandPrixClass * 8) + 0x57;
        if (LoadAsset(scaled + base_off, p) != 0) {
            register u8 *base_a0 asm("$4");
            u8 *base;
            s32 off0, off1;
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 0);
            UploadImageAsset(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 1);
            UploadImageAsset(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 2);
            UploadImageBlock(g_AssetBlockPtr);
            base = g_AssetLoadCursor;
            off0 = ((GameSceneAssetHeader *)base)->offsets[3];
            off1 = ((GameSceneAssetHeader *)base)->offsets[4];
            g_AssetBlockPtr = base + off0;
            g_AssetSubBlockPtr = base + off1;
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
        u8 *p;
        s32 scaled;
        register s32 result asm("$2");
        p = g_AssetLoadCursor;
        scaled = g_CourseIndex * 2;
        result = (g_GrandPrixClass * 8) + scaled;
        if (LoadAsset(result + 0x58, p) != 0) {
            register u8 *base_a0 asm("$4");
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 0); SetTrackCameraTable(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 1); SetEnvPaletteTable(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 2); SetEnvironmentScript(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 3); RegisterModelBank(g_AssetBlockPtr, 1);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 4); InstallTrackPoints(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 5); RegisterCourseModels(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 6); RegisterModelBank(g_AssetBlockPtr, 2);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 7); InstallTerrainCellData(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 8); SetCourseObjects(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 9); InstallTrackEventData(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 10); SelectTrackCameraTable(g_AssetBlockPtr, 1);
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
    s32 value;

    if (g_AssetLoadState == 1) {
        offset = g_GrandPrixSeries * 6;
        base = g_GrandPrixClass + 0x4A;
        value = LoadAsset((s32)(offset + base), (void *)g_ImageBlockBuffer);
        if (value != 0) {
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
    s32 value;

    if (g_AssetLoadState == 1) {
        s32 left = g_CourseIndex * 2;
        s32 right = (g_GrandPrixClass * 8) + 0x57;

        value = LoadAsset((s32)(left + right), (void *)g_AssetBase);
        if (value != 0) {
            g_AssetLoadState = 0;
            g_ImageBlockBuffer = value + g_AssetBase;
        }
    }
}
