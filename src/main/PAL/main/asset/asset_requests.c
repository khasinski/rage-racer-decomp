#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "psyq/gpu.h"
#include "game/cd.h"

extern u8 *g_AssetLoadCursor;
extern Rect g_TeamLogoClutRect;
extern Rect g_TeamLogoRect;
extern u16 g_TeamLogoClut[];
extern u16 g_TeamLogoCanvas[];
void UploadImageAsset(void *arg0);
void StartAudioSlotLoad(s32 arg0, void *arg1, void *arg2, s32 arg3);
extern s32 g_ImageBlockBuffer;
extern GameSceneAssetHeader *g_AssetBase;
extern void *g_AssetBlockPtr2;
extern void *g_AssetSubBlockPtr;
void CloseLoadedAudioSlots(void);

void LoadBootAssets(void) {
    u8 *loaded;
    u8 *base;
    register u8 *next asm("$2");
    s32 nextState;

    switch (g_AssetLoadState) {
    case 1:
        base = (u8 *)g_LoadBuffer;
        loaded = (u8 *)LoadAsset(1, base);
        if (loaded != 0) {
            UploadLoadBufferImage();
            next = loaded + (s32)base;
            g_AssetBlockPtr = next;
            g_AssetLoadState = 2;
        }
        break;
    case 2:
        loaded = (u8 *)LoadAsset(2, g_AssetBlockPtr);
        if (loaded != 0) {
            nextState = 3;
            next = g_AssetBlockPtr;
            goto setNextBuffer;
        }
        break;
    case 3:
        if (LoadAsset(3, g_AssetLoadCursor) != 0) {
            StartAudioSlotLoad(0, g_AssetBlockPtr, g_AssetLoadCursor, 0);
            g_AssetLoadState = 4;
        }
        break;
    case 4:
        if ((PollAudioSlotLoad() << 16) != 0) {
            g_AssetLoadState = 5;
        }
        break;
    case 5:
        loaded = (u8 *)LoadAsset(4, g_AssetLoadCursor);
        if (loaded != 0) {
            InstallResourceData(g_AssetLoadCursor);
            next = g_AssetLoadCursor;
            __asm__ volatile("" : "=r"(next) : "0"(next));
            nextState = 6;
setNextBuffer:
            g_AssetLoadState = nextState;
            __asm__ volatile("" ::: "memory");
            next = loaded + (s32)next;
            g_AssetLoadCursor = next;
        }
        break;
    case 6:
        if (LoadAsset(5, g_AssetLoadCursor) != 0) {
            u8 *finalBase;

            UploadImageAsset(g_AssetLoadCursor);
            StoreImage(&g_TeamLogoClutRect, g_TeamLogoClut);
            StoreImage(&g_TeamLogoRect, g_TeamLogoCanvas);
            DrawSync(0);
            finalBase = g_AssetLoadCursor;
            g_TeamLogoClut[0] = 0;
            g_AssetLoadState = 0;
            g_AssetBase = (GameSceneAssetHeader *)finalBase;
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
            g_ImageBlockBuffer = (s32)g_AssetBase;
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

    switch (g_AssetLoadState) {
    case 1:
        CloseLoadedAudioSlots();
        g_AssetLoadState = 2;
    case 2:
        if (LoadAsset(7, g_AssetBase) != 0) {
            header = g_AssetBase;
            firstOffset = *(volatile s32 *)&header->offsets[0];
            thirdOffset = *(volatile s32 *)&header->offsets[2];
            g_AssetBlockPtr = (void *)((u8 *)header + firstOffset);
            secondOffset = *(volatile s32 *)&header->offsets[1];
            g_AssetLoadState = 0;
            {
                s32 rel = secondOffset;
                secondOffset = (s32)header + rel;
            }
            header = (GameSceneAssetHeader *)((u8 *)header + thirdOffset);
            g_AssetBlockPtr2 = (void *)secondOffset;
            g_AssetSubBlockPtr = header;
        }
        break;
    }
}
