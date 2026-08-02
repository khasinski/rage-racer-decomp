#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"

extern u32 g_CarModelSlot asm("D_8009E87C");
extern GameCarModelAsset *g_CarModelAsset asm("D_8009E698");
extern GameAssetTripleHeader *g_AssetLoadCursor asm("D_8019CAFC");
extern s32 g_TeamLogoSampleData asm("D_8019CA64");
extern u8 *g_CarModelBuffer asm("D_801E4090");
extern u8 *g_ImageBlockBuffer asm("D_801E4B30");
extern u8 *g_AssetBlockPtr2 asm("D_8019C754");
extern u8 *g_AssetSubBlockPtr asm("D_801E8AB0");
s32 GetCarAssetIndex(s32 model, s32 grade) asm("func_80017848");
s32 func_80017C78(s32 assetIndex, void *dst);
void RegisterModelBank(void *arg0, s32 arg1) asm("func_80017948");
void RegisterCourseModels(void) asm("func_80017A6C");
void SetCarImageSlot(void *arg0, s32 arg1) asm("func_80017B44");
void SetCarModelSlot(void *arg0, s32 arg1) asm("func_80017B94");
void SelectCarModelSlot(s32 arg0) asm("func_80017BAC");
void UploadImageAsset(void *arg0) asm("func_8001A3C0");
void func_8001D748(s32 arg0, s32 arg1);
void func_8001D900(s32 arg0, s32 arg1);
void func_8005B768(s32 arg0, void *arg1, void *arg2, void *arg3);
s32 func_8005B89C(void);
void func_8005DBD8(void);
extern s32 g_PendingCarModelIndex asm("D_8009AEFC");
void ServiceAssetLoad(void) asm("func_80019C04");

s32 RequestCarSelectAssets(void) asm("func_80018530");
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

void LoadCarSelectAssets(void) asm("func_80018588");
void LoadCarSelectAssets(void) {
    s32 state = g_AssetLoadState;
    s32 state2;
    u8 *carModelBase;
    GameAssetTripleHeader *header;
    GameAssetTripleHeader *headerArg;
    GameCarModelAsset *model;
    register GameCarEntry *entry asm("$2");
    s32 carIndex;
    register s32 indexOffset asm("$2");
    s32 firstOffset;
    register s32 secondOffset asm("$4");
    s32 assetOffset;
    s32 modelPtr;

    state2 = 2;

    switch (state) {
    case 1:
        func_8005B768(1, g_AssetBlockPtr, g_AssetSubBlockPtr, g_AssetBlockPtr2);
        g_AssetLoadState = state2;
        return;
    case 2:
        if ((func_8005B89C() << 16) != 0) {
            func_8005DBD8();
            g_AssetLoadState = 3;
            g_AssetLoadCursor = (GameAssetTripleHeader *)g_AssetSubBlockPtr;
        }
        return;
    case 3:
            if (func_80017C78(8, g_AssetLoadCursor) != 0) {
                RegisterModelBank((u8 *)g_AssetLoadCursor + 0xC, 0xE);

                header = g_AssetLoadCursor;
                secondOffset = header->secondOffset;
                firstOffset = header->firstOffset;
                secondOffset = (s32)((u8 *)header + secondOffset);
                header = (GameAssetTripleHeader *)((u8 *)header + firstOffset);
                g_TeamLogoSampleData = (s32)header;
                g_AssetBlockPtr = (u8 *)secondOffset;
                RegisterCourseModels();

                headerArg = g_AssetLoadCursor;
                assetOffset = headerArg->thirdOffset;
                g_AssetBlockPtr = (u8 *)headerArg + assetOffset;
                UploadImageAsset(g_AssetBlockPtr);

                g_AssetLoadState = 4;
                g_CarModelBuffer = g_AssetBlockPtr;
                g_ImageBlockBuffer = g_AssetBlockPtr + 0x40000;
            }
        return;
    case 4:
            carIndex = g_PlayerCarIndex;
            indexOffset = carIndex << 3;
            entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
            assetOffset = GetCarAssetIndex(carIndex, entry->modelVariant) << 1;
            carModelBase = g_CarModelBuffer;

            if (func_80017C78(assetOffset + 0xA, carModelBase) != 0) {
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
                    indexOffset = carIndex << 3;
                    entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
                    func_8001D748(entry->paintColor1, g_CarModelAsset->imageDataOffset);
                    indexOffset = g_PlayerCarIndex << 3;
                    entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
                    func_8001D900(entry->paintColor2, g_CarModelAsset->imageDataOffset);
                }

                g_CarModelSlot = 0;
                g_AssetLoadState = 0;
            }
    }
    return;
}

s32 RequestCarModel(s32 arg0) asm("func_8001882C");
s32 RequestCarModel(s32 arg0) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 5;
    g_PendingCarModelIndex = arg0;
    g_AssetLoadState = 1;
    return 1;
}

void LoadCarModelNow(s32 arg0) asm("func_80018868");
void LoadCarModelNow(s32 arg0) {
    RequestCarModel(arg0);

    while (g_AssetLoadState != 0) {
        ServiceAssetLoad();
    }
}

void LoadCarModel(s32 arg0) asm("func_800188B8");
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

        if (func_80017C78(offset, ptr) != 0) {
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
                func_8001D748(((GameCarEntry *)entry)->paintColor1, ((GameCarModelAsset *)ptr)->imageDataOffset);
                entry = (u8 *)(index + (s32)g_CarTable);
                func_8001D900(((GameCarEntry *)entry)->paintColor2, ((GameCarModelAsset *)ptr)->imageDataOffset);
            }

            g_AssetLoadState = 0;
        }
    }
}

s32 RequestUpgradedCarModel(s32 arg0) asm("func_800189E4");
s32 RequestUpgradedCarModel(s32 arg0) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 6;
    g_PendingCarModelIndex = arg0;
    g_AssetLoadState = 1;
    return 1;
}

void LoadUpgradedCarModelNow(s32 arg0) asm("func_80018A20");
void LoadUpgradedCarModelNow(s32 arg0) {
    RequestUpgradedCarModel(arg0);

    while (g_AssetLoadState != 0) {
        ServiceAssetLoad();
    }
}
