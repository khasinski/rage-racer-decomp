#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/audio.h"

extern u32 g_CarModelSlot;
extern GameCarModelAsset *g_CarModelAsset;
extern GameSceneAssetHeader *g_AssetLoadCursor;
extern s32 g_TeamLogoSampleData;
extern u8 *g_ImageBlockBuffer;
extern u8 *g_AssetBlockPtr2;
extern u8 *g_AssetSubBlockPtr;
s32 GetCarAssetIndex(s32 model, s32 grade);
void RegisterCourseModels(void);
void SelectCarModelSlot(s32 arg0);
void UploadImageAsset(void *arg0);
void ApplyBodyColor1(s32 arg0, s32 arg1);
void ApplyBodyColor2(s32 arg0, s32 arg1);
void StartAudioSlotLoad(s32 arg0, void *arg1, void *arg2, void *arg3);

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
    s32 state2;
    u8 *carModelBase;
    GameSceneAssetHeader *header;
    GameSceneAssetHeader *headerArg;
    GameCarModelAsset *model;
    register GameCarEntry *entry asm("$2");
    s32 carIndex;
    s32 indexOffset;
    s32 firstOffset;
    register s32 secondOffset asm("$4");
    s32 assetOffset;
    s32 modelPtr;

    state2 = 2;

    switch (state) {
    case 1:
        StartAudioSlotLoad(1, g_AssetBlockPtr, g_AssetSubBlockPtr, g_AssetBlockPtr2);
        g_AssetLoadState = state2;
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

                headerArg = g_AssetLoadCursor;
                assetOffset = headerArg->offsets[2];
                g_AssetBlockPtr = (u8 *)headerArg + assetOffset;
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

s32 RequestCarModel(s32 arg0) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 5;
    g_PendingCarModelIndex = arg0;
    g_AssetLoadState = 1;
    return 1;
}

void LoadCarModelNow(s32 arg0) {
    RequestCarModel(arg0);

    while (g_AssetLoadState != 0) {
        ServiceAssetLoad();
    }
}

void LoadCarModel(s32 arg0) {
    u8 *ptr;
    s32 index;
    register s32 arg asm("$17");
    s32 offset;

    arg = arg0;
    index = arg << 3;
    offset = (GetCarAssetIndex(arg, ((GameCarEntry *)(index + (s32)g_CarTable))->modelVariant) * 2) + 0xA;

    if (g_AssetLoadState == 1) {
        ptr = g_CarModelBuffer;
        if (g_CarModelSlot == 0) {
            ptr += 0x20000;
        }

        if (LoadAsset(offset, ptr) != 0) {
            s32 fixed;
            u32 flag;
            register s32 test asm("$2");
            u8 *entry;

            SetCarModelSlot(ptr, g_CarModelSlot < 1);
            fixed = *(volatile s32 *)(ptr + 0x20);
            flag = g_CarModelSlot;
            {
                s32 rel = fixed;
                fixed = (s32)ptr + rel;
            }
            flag = flag < 1;
            ((GameCarModelAsset *)ptr)->modelDataOffset = fixed;
            RegisterModelBank((void *)fixed, flag);
            fixed = *(volatile s32 *)(ptr + 0x24);
            flag = g_CarModelSlot;
            {
                s32 rel = fixed;
                fixed = (s32)ptr + rel;
            }
            flag = flag < 1;
            ((GameCarModelAsset *)ptr)->imageDataOffset = fixed;
            SetCarImageSlot((void *)fixed, flag);

            test = arg < 10;
            if (test != 0) {
                entry = (u8 *)(index + (s32)g_CarTable);
                ApplyBodyColor1(((GameCarEntry *)entry)->paintColor1, ((GameCarModelAsset *)ptr)->imageDataOffset);
                entry = (u8 *)(index + (s32)g_CarTable);
                ApplyBodyColor2(((GameCarEntry *)entry)->paintColor2, ((GameCarModelAsset *)ptr)->imageDataOffset);
            }

            g_AssetLoadState = 0;
        }
    }
}

s32 RequestUpgradedCarModel(s32 arg0) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 6;
    g_PendingCarModelIndex = arg0;
    g_AssetLoadState = 1;
    return 1;
}

void LoadUpgradedCarModelNow(s32 arg0) {
    RequestUpgradedCarModel(arg0);

    while (g_AssetLoadState != 0) {
        ServiceAssetLoad();
    }
}
