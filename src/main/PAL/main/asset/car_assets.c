#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/audio.h"

/* Asset-region pointers, each spelled with this file's own view of the
 * pointee; game/asset.h explains why they are not one shared declaration. */
extern u32 g_CarModelSlot;
extern GameCarModelAsset *g_CarModelAsset;
extern GameSceneAssetHeader *g_AssetLoadCursor;
extern s32 g_TeamLogoSampleData;
extern u8 *g_ImageBlockBuffer;
extern u8 *g_AssetBlockPtr2;
extern u8 *g_AssetSubBlockPtr;
void RegisterCourseModels(void);
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
    s32 nextState;
    u8 *carModelBase;
    GameSceneAssetHeader *header;
    GameSceneAssetHeader *imageHeader;
    GameCarModelAsset *model;
    register GameCarEntry *entry asm("$2");
    s32 carIndex;
    s32 indexOffset;
    s32 firstOffset;
    register s32 secondOffset asm("$4");
    s32 assetOffset;
    s32 modelPtr;

    nextState = 2;

    switch (state) {
    case 1:
        StartAudioSlotLoad(1, g_AssetBlockPtr, g_AssetSubBlockPtr, g_AssetBlockPtr2);
        g_AssetLoadState = nextState;
        return;
    case 2:
        if ((PollAudioSlotLoad() << 16) != 0) {
            InitSequenceAudio();
            g_AssetLoadState = 3;
            g_AssetLoadCursor = (GameSceneAssetHeader *)g_AssetSubBlockPtr;
        }
        return;
    case 3:
            if (LoadAsset(8, g_AssetLoadCursor) != 0) {
                RegisterModelBank((u8 *)g_AssetLoadCursor + 0xC, 0xE);

                header = g_AssetLoadCursor;
                secondOffset = header->offsets[1];
                firstOffset = header->offsets[0];
                secondOffset = (s32)((u8 *)header + secondOffset);
                header = (GameSceneAssetHeader *)((u8 *)header + firstOffset);
                g_TeamLogoSampleData = (s32)header;
                g_AssetBlockPtr = (u8 *)secondOffset;
                RegisterCourseModels();

                imageHeader = g_AssetLoadCursor;
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
            indexOffset = carIndex * 8;
            entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
            assetOffset = GetCarAssetIndex(carIndex, entry->modelVariant) << 1;
            carModelBase = g_CarModelBuffer;

            if (LoadAsset(assetOffset + 0xA, carModelBase) != 0) {
                SetCarModelSlot(carModelBase, 0);
                SelectCarModelSlot(0);

                model = g_CarModelAsset;
                modelPtr = model->modelDataOffset;
                {
                    s32 rel = modelPtr;
                    modelPtr = (s32)(carModelBase + rel);
                }
                model->modelDataOffset = modelPtr;
                RegisterModelBank((void *)modelPtr, 0);

                model = g_CarModelAsset;
                modelPtr = model->imageDataOffset;
                {
                    s32 rel = modelPtr;
                    modelPtr = (s32)(carModelBase + rel);
                }
                model->imageDataOffset = modelPtr;
                SetCarImageSlot((void *)modelPtr, 0);

                carIndex = g_PlayerCarIndex;
                if (carIndex < 10) {
                    indexOffset = carIndex * 8;
                    entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
                    ApplyBodyColor1(entry->paintColor1, g_CarModelAsset->imageDataOffset);
                    indexOffset = g_PlayerCarIndex * 8;
                    entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
                    ApplyBodyColor2(entry->paintColor2, g_CarModelAsset->imageDataOffset);
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
    u8 *dst;
    s32 carOffset;
    register s32 car asm("$17");
    s32 assetIndex;

    car = carIndex;
    carOffset = car << 3;
    assetIndex = (GetCarAssetIndex(car, ((GameCarEntry *)(carOffset + (s32)g_CarTable))->modelVariant) * 2) + 0xA;

    if (g_AssetLoadState == 1) {
        dst = g_CarModelBuffer;
        if (g_CarModelSlot == 0) {
            dst += 0x20000;
        }

        if (LoadAsset(assetIndex, dst) != 0) {
            s32 addr;
            u32 slot;
            register s32 paintable asm("$2");
            u8 *carEntry;

            SetCarModelSlot(dst, g_CarModelSlot < 1);
            addr = *(volatile s32 *)(dst + 0x20);
            slot = g_CarModelSlot;
            {
                s32 rel = addr;
                addr = (s32)dst + rel;
            }
            slot = slot < 1;
            ((GameCarModelAsset *)dst)->modelDataOffset = addr;
            RegisterModelBank((void *)addr, slot);
            addr = *(volatile s32 *)(dst + 0x24);
            slot = g_CarModelSlot;
            {
                s32 rel = addr;
                addr = (s32)dst + rel;
            }
            slot = slot < 1;
            ((GameCarModelAsset *)dst)->imageDataOffset = addr;
            SetCarImageSlot((void *)addr, slot);

            paintable = car < 10;
            if (paintable != 0) {
                carEntry = (u8 *)(carOffset + (s32)g_CarTable);
                ApplyBodyColor1(((GameCarEntry *)carEntry)->paintColor1, ((GameCarModelAsset *)dst)->imageDataOffset);
                carEntry = (u8 *)(carOffset + (s32)g_CarTable);
                ApplyBodyColor2(((GameCarEntry *)carEntry)->paintColor2, ((GameCarModelAsset *)dst)->imageDataOffset);
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
