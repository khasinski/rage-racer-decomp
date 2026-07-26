#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"

extern u32 g_CarModelSlot asm("D_8009E87C");
extern GameCarModelAsset *D_8009E698;
extern GameAssetTripleHeader *D_8019CAFC;
extern s32 D_8019CA64;
extern u8 *D_801E4090;
extern u8 *D_801E4B30;
extern u8 *D_8019C754;
extern u8 *D_801E8AB0;
s32 GameGetCarAssetIndex(s32 model, s32 grade) asm("func_80017848");
s32 func_80017C78(s32 assetIndex, void *dst);
void GameRegisterModelBank(void *arg0, s32 arg1) asm("func_80017948");
void func_80017A6C(void);
void func_80017B44(void *arg0, s32 arg1);
void func_80017B94(void *arg0, s32 arg1);
void func_80017BAC(s32 arg0);
void GameUploadImageAsset(void *arg0) asm("func_8001A3C0");
void func_8001D748(s32 arg0, s32 arg1);
void func_8001D900(s32 arg0, s32 arg1);
void func_8005B768(s32 arg0, void *arg1, void *arg2, void *arg3);
s32 func_8005B89C(void);
void func_8005DBD8(void);
extern s32 D_8009AEFC;
void func_80019C04(void);

s32 func_80018530(void) {
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

void func_80018588(void) {
    register s32 state asm("$3") = g_AssetLoadState;
    register s32 state2 asm("$16");
    register u8 *carModelBase asm("$16");
    register GameAssetTripleHeader *header asm("$2");
    register GameAssetTripleHeader *headerArg asm("$4");
    register GameCarModelAsset *model asm("$2");
    register GameCarEntry *entry asm("$2");
    register s32 carIndex asm("$4");
    register s32 indexOffset asm("$2");
    register s32 firstOffset asm("$3");
    register s32 secondOffset asm("$4");
    register s32 assetOffset asm("$2");
    register s32 modelPtr asm("$4");

    state2 = 2;

    if (state == state2) {
        goto state_2;
    }
    if (state < 3) {
        if (state == 1) {
            goto state_1;
        }
        goto done;
    }
    if (state == 3) {
        goto state_3;
    }
    if (state == 4) {
        goto state_4;
    }
    goto done;

state_1:
        __asm__ volatile("" ::: "$3");
        func_8005B768(1, g_AssetBlockPtr, D_801E8AB0, D_8019C754);
        g_AssetLoadState = state2;
        goto done;
state_2:
        if ((func_8005B89C() << 16) != 0) {
            func_8005DBD8();
            g_AssetLoadState = 3;
            D_8019CAFC = (GameAssetTripleHeader *)D_801E8AB0;
        }
        goto done;
state_3:
            if (func_80017C78(8, D_8019CAFC) != 0) {
                GameRegisterModelBank((u8 *)D_8019CAFC + 0xC, 0xE);

                header = D_8019CAFC;
                secondOffset = header->secondOffset;
                firstOffset = header->firstOffset;
                secondOffset = (s32)((u8 *)header + secondOffset);
                header = (GameAssetTripleHeader *)((u8 *)header + firstOffset);
                D_8019CA64 = (s32)header;
                g_AssetBlockPtr = (u8 *)secondOffset;
                func_80017A6C();

                headerArg = D_8019CAFC;
                assetOffset = headerArg->thirdOffset;
                g_AssetBlockPtr = (u8 *)headerArg + assetOffset;
                GameUploadImageAsset(g_AssetBlockPtr);

                g_AssetLoadState = 4;
                D_801E4090 = g_AssetBlockPtr;
                D_801E4B30 = g_AssetBlockPtr + 0x40000;
            }
        goto done;
state_4:
            carIndex = g_PlayerCarIndex;
            indexOffset = carIndex << 3;
            entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
            assetOffset = GameGetCarAssetIndex(carIndex, entry->modelVariant) << 1;
            carModelBase = D_801E4090;

            if (func_80017C78(assetOffset + 0xA, carModelBase) != 0) {
                func_80017B94(carModelBase, 0);
                func_80017BAC(0);

                model = D_8009E698;
                modelPtr = model->modelDataOffset;
                modelPtr = (s32)(carModelBase + modelPtr);
                model->modelDataOffset = modelPtr;
                GameRegisterModelBank((void *)modelPtr, 0);

                model = D_8009E698;
                modelPtr = model->imageDataOffset;
                modelPtr = (s32)(carModelBase + modelPtr);
                model->imageDataOffset = modelPtr;
                func_80017B44((void *)modelPtr, 0);

                carIndex = g_PlayerCarIndex;
                if (carIndex < 10) {
                    indexOffset = carIndex << 3;
                    entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
                    func_8001D748(entry->shapeIndex, D_8009E698->imageDataOffset);
                    indexOffset = g_PlayerCarIndex << 3;
                    entry = (GameCarEntry *)(indexOffset + (s32)g_CarTable);
                    func_8001D900(entry->textureIndex, D_8009E698->imageDataOffset);
                }

                g_CarModelSlot = 0;
                g_AssetLoadState = 0;
            }
done:
    return;
}

s32 func_8001882C(s32 arg0) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 5;
    D_8009AEFC = arg0;
    g_AssetLoadState = 1;
    return 1;
}

void func_80018868(s32 arg0) {
    func_8001882C(arg0);

    while (g_AssetLoadState != 0) {
        func_80019C04();
    }
}

void func_800188B8(s32 arg0) {
    register u8 *ptr asm("$16");
    register s32 index asm("$18");
    register s32 arg asm("$17");
    s32 offset;

    arg = arg0;
    index = arg << 3;
    offset = (GameGetCarAssetIndex(arg, ((GameCarEntry *)(index + (s32)g_CarTable))->modelVariant) * 2) + 0xA;

    if (g_AssetLoadState == 1) {
        ptr = D_801E4090;
        if (g_CarModelSlot == 0) {
            ptr += 0x20000;
        }

        if (func_80017C78(offset, ptr) != 0) {
            register s32 fixed asm("$4");
            register u32 flag asm("$5");
            register s32 test asm("$2");
            register u8 *entry asm("$2");

            func_80017B94(ptr, g_CarModelSlot < 1);
            fixed = *(volatile s32 *)(ptr + 0x20);
            flag = g_CarModelSlot;
            fixed = (s32)ptr + fixed;
            flag = flag < 1;
            ((GameCarModelAsset *)ptr)->modelDataOffset = fixed;
            GameRegisterModelBank((void *)fixed, flag);
            fixed = *(volatile s32 *)(ptr + 0x24);
            flag = g_CarModelSlot;
            fixed = (s32)ptr + fixed;
            flag = flag < 1;
            ((GameCarModelAsset *)ptr)->imageDataOffset = fixed;
            func_80017B44((void *)fixed, flag);

            test = arg < 10;
            if (test != 0) {
                entry = (u8 *)(index + (s32)g_CarTable);
                func_8001D748(((GameCarEntry *)entry)->shapeIndex, ((GameCarModelAsset *)ptr)->imageDataOffset);
                entry = (u8 *)(index + (s32)g_CarTable);
                func_8001D900(((GameCarEntry *)entry)->textureIndex, ((GameCarModelAsset *)ptr)->imageDataOffset);
            }

            g_AssetLoadState = 0;
        }
    }
}

s32 func_800189E4(s32 arg0) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 6;
    D_8009AEFC = arg0;
    g_AssetLoadState = 1;
    return 1;
}

void func_80018A20(s32 arg0) {
    func_800189E4(arg0);

    while (g_AssetLoadState != 0) {
        func_80019C04();
    }
}
