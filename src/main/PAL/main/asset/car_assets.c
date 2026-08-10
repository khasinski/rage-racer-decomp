#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/asset_internal.h"
#include "game/car.h"
#include "game/audio.h"

/* g_CarModelAsset really is per-screen typed; see game/asset.h.
 * g_TeamLogoSampleData's other reader, menu/team_logo.c, walks it through a
 * TeamLogoSample struct private to that file. */

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
    CarModelAsset *model;
    s32 carIndex;
    s32 firstOffset;
    u8 *secondBlock;
    s32 blockOffset;
    s32 assetOffset;
    void *modelPtr;
    s32 relOffset;

    switch (state) {
    case 1:
        StartAudioSlotLoad(1, g_AssetBlockPtr, g_AssetSubBlockPtr,
                           (u16 *)g_AssetBlockPtr2);
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
                RegisterModelBank((ModelBankHeader *)(g_AssetLoadCursor + 0xC), 0xE);

                header = GetSceneAssetHeader(g_AssetLoadCursor);
                blockOffset = header->offsets[1];
                firstOffset = header->offsets[0];
                secondBlock = GetSceneAssetAddress(header, blockOffset);
                header = GetSceneAssetAddress(header, firstOffset);
                g_TeamLogoSampleData = (TeamLogoSample *)header;
                g_AssetBlockPtr = secondBlock;
                RegisterCourseModels((CourseModelAssetHeader *)g_AssetBlockPtr);

                imageHeader = GetSceneAssetHeader(g_AssetLoadCursor);
                assetOffset = imageHeader->offsets[2];
                g_AssetBlockPtr = GetSceneAssetAddress(imageHeader, assetOffset);
                UploadImageAsset(g_AssetBlockPtr);

                g_AssetLoadState = 4;
                g_CarModelBuffer = g_AssetBlockPtr;
                g_ImageBlockBuffer = g_AssetBlockPtr + CAR_MODEL_BUFFER_SIZE;
            }
        return;
    case 4:
            carIndex = g_PlayerCarIndex;
            assetOffset = GetCarAssetIndex(carIndex, g_CarTable[carIndex].modelVariant) << 1;
            carModelBase = g_CarModelBuffer;

            if (LoadAsset(assetOffset + 0xA, carModelBase) != 0) {
                SetCarModelSlot((CarModelAsset *)carModelBase, 0);
                SelectCarModelSlot(0);

                model = g_CarModelAsset;
                relOffset = model->modelData.offset;
                modelPtr = carModelBase + relOffset;
                model->modelData.pointer = modelPtr;
                RegisterModelBank((ModelBankHeader *)modelPtr, 0);

                model = g_CarModelAsset;
                relOffset = model->imageData.offset;
                modelPtr = carModelBase + relOffset;
                model->imageData.pointer = modelPtr;
                SetCarImageSlot((CarImageData *)modelPtr, 0);

                carIndex = g_PlayerCarIndex;
                if (carIndex < 10) {
                    ApplyBodyColor1(g_CarTable[carIndex].paintColor1,
                                    (CarImageData *)g_CarModelAsset->imageData.pointer);
                    ApplyBodyColor2(g_CarTable[g_PlayerCarIndex].paintColor2,
                                    (CarImageData *)g_CarModelAsset->imageData.pointer);
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
    CarModelAsset *asset;
    s32 assetId;

    assetId = (GetCarAssetIndex(carIndex, g_CarTable[carIndex].modelVariant) * 2) + 0xA;

    if (g_AssetLoadState == 1) {
        ptr = g_CarModelBuffer;
        if (g_CarModelSlot == 0) {
            ptr += CAR_MODEL_SLOT_SIZE;
        }

        if (LoadAsset(assetId, ptr) != 0) {
            asset = (CarModelAsset *)ptr;
            SetCarModelSlot((CarModelAsset *)ptr, g_CarModelSlot < 1);

            asset->modelData.pointer = ptr + asset->modelData.offset;
            RegisterModelBank((ModelBankHeader *)asset->modelData.pointer,
                              g_CarModelSlot < 1);

            asset->imageData.pointer = ptr + asset->imageData.offset;
            SetCarImageSlot((CarImageData *)asset->imageData.pointer,
                            g_CarModelSlot < 1);

            if (carIndex < 10) {
                ApplyBodyColor1(g_CarTable[carIndex].paintColor1,
                                (CarImageData *)asset->imageData.pointer);
                ApplyBodyColor2(g_CarTable[carIndex].paintColor2,
                                (CarImageData *)asset->imageData.pointer);
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
