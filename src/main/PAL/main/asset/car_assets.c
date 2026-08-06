#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/audio.h"

/* Asset-region pointers, each spelled with this file's own view of the
 * pointee; game/asset.h explains why they are not one shared declaration. */
extern u32 g_CarModelSlot;
extern GameCarModelAsset *g_CarModelAsset;
extern s32 g_TeamLogoSampleData;
extern u8 *g_ImageBlockBuffer;
extern u8 *g_AssetBlockPtr2;
void RegisterCourseModels(void *base);
void SelectCarModelSlot(s32 slot);
void UploadImageAsset(void *asset);
void ApplyBodyColor1(s32 colour, s32 imageData);
void ApplyBodyColor2(s32 colour, s32 imageData);
void StartAudioSlotLoad(s32 slot, void *header, void *body, void *table);

s32 RequestCarSelectAssets(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 4) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 4;
    g_AssetLoadState = 1;
    return 1;
}

void LoadCarSelectAssets(void) {
    s32 state = g_AssetLoadState;
    u8 *carModelBase;
    GameSceneAssetHeader *header;
    GameSceneAssetHeader *imageHeader;
    GameCarModelAsset *model;
    s32 carIndex;
    s32 firstOffset;
    s32 secondOffset;
    s32 blockOffset;
    s32 assetOffset;
    s32 modelPtr;
    s32 relOffset;

    switch (state) {
    case 1:
        StartAudioSlotLoad(1, g_AssetBlockPtr, g_AssetSubBlockPtr, g_AssetBlockPtr2);
        g_AssetLoadState = 2;
        return;
    case 2:
        if ((s16)PollAudioSlotLoad() != 0) {
            InitSequenceAudio();
            g_AssetLoadState = 3;
            g_AssetLoadCursor = g_AssetSubBlockPtr;
        }
        return;
    case 3:
            if (LoadAsset(8, g_AssetLoadCursor) != 0) {
                RegisterModelBank(g_AssetLoadCursor + 0xC, 0xE);

                header = (GameSceneAssetHeader *)g_AssetLoadCursor;
                blockOffset = header->offsets[1];
                firstOffset = header->offsets[0];
                secondOffset = (s32)((u8 *)header + blockOffset);
                header = (GameSceneAssetHeader *)((u8 *)header + firstOffset);
                g_TeamLogoSampleData = (s32)header;
                g_AssetBlockPtr = (u8 *)secondOffset;
                RegisterCourseModels(g_AssetBlockPtr);

                imageHeader = (GameSceneAssetHeader *)g_AssetLoadCursor;
                assetOffset = imageHeader->offsets[2];
                g_AssetBlockPtr = (u8 *)imageHeader + assetOffset;
                UploadImageAsset(g_AssetBlockPtr);

                g_AssetLoadState = 4;
                g_CarModelBuffer = g_AssetBlockPtr;
                g_ImageBlockBuffer = g_AssetBlockPtr + 0x40000;
            }
        return;
    case 4:
            carIndex = g_PlayerCarIndex;
            assetOffset = GetCarAssetIndex(carIndex, g_CarTable[carIndex].modelVariant) << 1;
            carModelBase = g_CarModelBuffer;

            if (LoadAsset(assetOffset + 0xA, carModelBase) != 0) {
                SetCarModelSlot(carModelBase, 0);
                SelectCarModelSlot(0);

                model = g_CarModelAsset;
                relOffset = model->modelDataOffset;
                modelPtr = (s32)(carModelBase + relOffset);
                model->modelDataOffset = modelPtr;
                RegisterModelBank((void *)modelPtr, 0);

                model = g_CarModelAsset;
                relOffset = model->imageDataOffset;
                modelPtr = (s32)(carModelBase + relOffset);
                model->imageDataOffset = modelPtr;
                SetCarImageSlot((void *)modelPtr, 0);

                carIndex = g_PlayerCarIndex;
                if (carIndex < 10) {
                    ApplyBodyColor1(g_CarTable[carIndex].paintColor1, g_CarModelAsset->imageDataOffset);
                    ApplyBodyColor2(g_CarTable[g_PlayerCarIndex].paintColor2, g_CarModelAsset->imageDataOffset);
                }

                g_CarModelSlot = 0;
                g_AssetLoadState = 0;
            }
    }
    return;
}

s32 RequestCarModel(s32 carIndex) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 5;
    g_PendingCarModelIndex = carIndex;
    g_AssetLoadState = 1;
    return 1;
}

void LoadCarModelNow(s32 carIndex) {
    RequestCarModel(carIndex);

    while (g_AssetLoadState != 0) {
        ServiceAssetLoad();
    }
}

void LoadCarModel(s32 carIndex) {
    u8 *ptr;
    GameCarModelAsset *asset;
    s32 assetId;

    assetId = (GetCarAssetIndex(carIndex, g_CarTable[carIndex].modelVariant) * 2) + 0xA;

    if (g_AssetLoadState == 1) {
        ptr = g_CarModelBuffer;
        if (g_CarModelSlot == 0) {
            ptr += 0x20000;
        }

        if (LoadAsset(assetId, ptr) != 0) {
            asset = (GameCarModelAsset *)ptr;
            SetCarModelSlot(ptr, g_CarModelSlot < 1);

            asset->modelDataOffset = (s32)ptr + asset->modelDataOffset;
            RegisterModelBank((void *)asset->modelDataOffset, g_CarModelSlot < 1);

            asset->imageDataOffset = (s32)ptr + asset->imageDataOffset;
            SetCarImageSlot((void *)asset->imageDataOffset, g_CarModelSlot < 1);

            if (carIndex < 10) {
                ApplyBodyColor1(g_CarTable[carIndex].paintColor1, asset->imageDataOffset);
                ApplyBodyColor2(g_CarTable[carIndex].paintColor2, asset->imageDataOffset);
            }

            g_AssetLoadState = 0;
        }
    }
}

s32 RequestUpgradedCarModel(s32 carIndex) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 6;
    g_PendingCarModelIndex = carIndex;
    g_AssetLoadState = 1;
    return 1;
}

void LoadUpgradedCarModelNow(s32 carIndex) {
    RequestUpgradedCarModel(carIndex);

    while (g_AssetLoadState != 0) {
        ServiceAssetLoad();
    }
}
