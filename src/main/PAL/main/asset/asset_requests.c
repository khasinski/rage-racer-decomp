#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "psyq/gpu.h"
#include "game/cd.h"
#include "game/audio.h"

/* Asset-region pointers, each spelled with this file's own view of the
 * pointee; game/asset.h explains why they are not one shared declaration. */
extern Rect g_TeamLogoClutRect;
extern Rect g_TeamLogoRect;
extern u16 g_TeamLogoClut[];
extern u16 g_TeamLogoCanvas[];

void LoadBootAssets(void) {
    u8 *loaded;
    u8 *base;

    switch (g_AssetLoadState) {
    case 1:
        base = (u8 *)g_LoadBuffer;
        loaded = (u8 *)LoadAsset(1, base);
        if (loaded != 0) {
            UploadLoadBufferImage();
            g_AssetBlockPtr = loaded + (s32)base;
            g_AssetLoadState = 2;
        }
        break;
    case 2:
        loaded = (u8 *)LoadAsset(2, g_AssetBlockPtr);
        if (loaded != 0) {
            g_AssetLoadState = 3;
            g_AssetLoadCursor = loaded + (s32)g_AssetBlockPtr;
        }
        break;
    case 3:
        if (LoadAsset(3, g_AssetLoadCursor) != 0) {
            StartAudioSlotLoad(0, (s32)g_AssetBlockPtr, (s32)g_AssetLoadCursor, 0);
            g_AssetLoadState = 4;
        }
        break;
    case 4:
        if ((s16)PollAudioSlotLoad() != 0) {
            g_AssetLoadState = 5;
        }
        break;
    case 5:
        loaded = (u8 *)LoadAsset(4, g_AssetLoadCursor);
        if (loaded != 0) {
            InstallResourceData(g_AssetLoadCursor);
            g_AssetLoadState = 6;
            g_AssetLoadCursor = loaded + (s32)g_AssetLoadCursor;
        }
        break;
    case 6:
        if (LoadAsset(5, g_AssetLoadCursor) != 0) {
            u8 *assetBase;

            UploadImageAsset(g_AssetLoadCursor);
            StoreImage(&g_TeamLogoClutRect, g_TeamLogoClut);
            StoreImage(&g_TeamLogoRect, g_TeamLogoCanvas);
            DrawSync(0);
            assetBase = g_AssetLoadCursor;
            g_TeamLogoClut[0] = 0;
            g_AssetLoadState = 0;
            g_AssetBase = assetBase;
        }
        break;
    }
}

s32 RequestSaveScreenAssets(void) {
    s32 state;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    state = 2;
    if (g_MainState == state) {
        g_MainState = 0;
        return 0;
    }

    ResetCdAudioState();
    g_MainState = state;
    g_AssetLoadState = 1;
    return 1;
}

void LoadSaveScreenAssets(void) {
    if (g_AssetLoadState == 1) {
        if (LoadAsset(6, g_AssetBase) != 0) {
            g_AssetLoadState = 0;
            g_ImageBlockBuffer = g_AssetBase;
        }
    }
}

s32 RequestSelectBgmAssetsNoReset(void) {
    s32 loadType;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    loadType = 3;
    if (g_MainState == loadType) {
        g_MainState = 0;
        return 0;
    }

    ResetCdAudioState();
    g_MainState = loadType;
    g_AssetLoadState = 2;
    return 1;
}

s32 RequestSelectBgmAssets(void) {
    s32 loadType;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    loadType = 3;
    if (g_MainState == loadType) {
        g_MainState = 0;
        return 0;
    }

    ResetCdAudioState();
    g_MainState = loadType;
    g_AssetLoadState = 1;
    return 1;
}

void LoadSelectBgmAssets(void) {
    GameSceneAssetHeader *header;
    s32 firstOffset;
    s32 secondOffset;
    s32 thirdOffset;
    s32 relOffset;

    switch (g_AssetLoadState) {
    case 1:
        CloseLoadedAudioSlots();
        g_AssetLoadState = 2;
    case 2:
        if (LoadAsset(7, g_AssetBase) != 0) {
            header = (GameSceneAssetHeader *)g_AssetBase;
            /* The three volatile reads are load-bearing: without them cse
             * folds the header pointer and the three offsets into one
             * addressing pattern and the block costs 19 instructions more. */
            firstOffset = *(volatile s32 *)&header->offsets[0];
            thirdOffset = *(volatile s32 *)&header->offsets[2];
            g_AssetBlockPtr = (void *)((u8 *)header + firstOffset);
            relOffset = *(volatile s32 *)&header->offsets[1];
            g_AssetLoadState = 0;
            secondOffset = (s32)header + relOffset;
            header = (GameSceneAssetHeader *)((u8 *)header + thirdOffset);
            g_AssetBlockPtr2 = (u8 *)secondOffset;
            g_AssetSubBlockPtr = (u8 *)header;
        }
        break;
    }
}
