#include "common.h"
#include "game/asset.h"
#include "game/asset_internal.h"
#include "game/car.h"
#include "game/state.h"
#include "game/race.h"
#include "game/render.h"
#include "game/cd.h"
#include "game/random.h"


/* Where asset 0x56 lands: g_ImageBlockBuffer advanced past the car texture
 * block just loaded. Its header words 1 and 2 are relocated into
 * g_AssetBlockPtr / g_AssetSubBlockPtr and word 0 is kept as-is. */

void LoadUpgradedCarModel(s32 carIndex) {
    u8 *ptr;
    CarModelAsset *asset;
    s32 offset;
    s32 assetId;
    u32 mode;

    if (g_AssetLoadState == 1) {
        offset = GetCarAssetIndex(carIndex, g_CarTable[carIndex].modelVariant + 1) << 1;
        mode = g_CarModelSlot;
        ptr = g_CarModelBuffer;
        assetId = offset + 0xA;

        if (mode == 0) {
            ptr += 0x20000;
        }

        if (LoadAsset(assetId, ptr) != 0) {
            asset = (CarModelAsset *)ptr;
            SetCarModelSlot(asset, g_CarModelSlot < 1);

            asset->modelData.pointer = ptr + asset->modelData.offset;
            RegisterModelBank(asset->modelData.modelBank, g_CarModelSlot < 1);

            asset->imageData.pointer = ptr + asset->imageData.offset;
            SetCarImageSlot(asset->imageData.carImage, g_CarModelSlot < 1);

            if (g_PlayerCarIndex < 10) {
                ApplyBodyColor1(g_CarTable[carIndex].paintColor1,
                                asset->imageData.carImage);
                ApplyBodyColor2(g_CarTable[carIndex].paintColor2,
                                asset->imageData.carImage);
            }

            g_AssetLoadState = 0;
        }
    }
}

s32 RequestOptionScreenAssets(void) {
    s32 state;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    state = 7;
    if (g_MainState == state) {
        g_MainState = 0;
        return 0;
    }

    ResetCdAudioState();
    g_MainState = state;
    g_AssetLoadState = 1;
    return 1;
}

void LoadOptionScreenAssets(void) {
    u8 *base;
    s32 offset;

    if (g_AssetLoadState == 1) {
        if (LoadAsset(9, g_AssetBase) != 0) {
            RegisterModelBank((ModelBankHeader *)(g_AssetBase + 4), 0);
            SelectModelBank(0);

            base = g_AssetBase;
            offset = *(s32 *)base;
            g_AssetLoadState = 0;
            g_ImageBlockBuffer = base + offset;
        }
    }
}

s32 RequestRoundAssets(void) {
    s32 value;

    if (g_AssetLoadState != 0) {
        ResetAssetLoader();
    }

    if (g_GrandPrixMode == 0) {
        value = (Random15() & 0xFFF) % (g_MaxClassReached[g_GrandPrixSeries] + 1);
        g_GrandPrixClass = value;
        if (((g_CourseIndex & 3) == 3) && (value < 2)) {
            g_GrandPrixClass = ((Random15() & 0xFFF) % (g_MaxClassReached[g_GrandPrixSeries] - 1)) + 2;
        }
    }

    if (g_MainState == 8) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 8;
    g_AssetLoadState = 1;
    return 1;
}

void LoadRoundAssets(void) {
    s32 state;
    s32 kind;
    s32 result;

    state = g_AssetLoadState;
    switch (state) {
    case 1:
        kind = 0x55;
        if (g_GrandPrixMode != 0) {
            register s32 index asm("$2") = g_GrandPrixSeries;
            s32 scaled;

            scaled = index << 1;
            scaled += index;
            index = g_GrandPrixClass;
            scaled <<= 1;
            index += 0x4A;
            kind = scaled + index;
        }

        result = LoadAsset(kind, g_ImageBlockBuffer);
        if (result != 0) {
            g_AssetLoadState = 2;
            g_AssetBlockPtr2 = g_ImageBlockBuffer + result;
        }
        break;
    case 2:
        if (LoadAsset(0x56, g_AssetBlockPtr2) != 0) {
            u8 *header;
            u8 *first;
            u8 *second;
            s32 shared;

            header = g_AssetBlockPtr2;
            first = header + ((s32 *)header)[1];
            second = header + ((s32 *)header)[2];
            g_AssetBlockPtr = first;
            g_AssetSubBlockPtr = second;
            shared = *(s32 *)header;
            g_AssetLoadState = 0;
            g_SharedAssetWord0 = shared;
        }
        break;
    }
}

void RelocateCarModel(void) {
    AssetAddress address;
    u32 *dst;
    u32 *src;
    u32 count;

    address.pointer = g_CarModelAsset;
    if (address.offset != 0) {
        src = address.pointer;
    } else {
        src = address.pointer;
    }
    count = src[6];
    address.pointer = g_AssetBase;
    if (count != 0) {
        dst = address.pointer;
    } else {
        dst = address.pointer;
    }
    count = count + 0x28;
    address.pointer = dst;
    address.offset = count + address.offset;
    count >>= 2;
    g_AssetLoadCursor = address.pointer;

    while (count != 0) {
        *dst = *src;
        src++;
        count--;
        dst++;
    }

    SetCarModelSlot((CarModelAsset *)g_AssetBase, 0);
    address.pointer = g_CarModelAsset->modelData.pointer;
    UnrelocateModelBank((ModelBankHeader *)(g_AssetBase + 0x28), address.offset);
    SelectCarModelSlot(0);
    g_CarModelAsset->modelData.pointer = g_AssetBase + 0x28;
    RegisterModelBank((ModelBankHeader *)(g_AssetBase + 0x28), 0);
}
