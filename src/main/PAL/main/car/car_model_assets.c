#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/state.h"
#include "game/race.h"
#include "game/render.h"
#include "game/cd.h"
#include "game/random.h"

extern u8 *g_CarModelAsset;

extern u32 g_CarModelSlot;
s32 GetCarAssetIndex(s32 model, s32 grade);
void ApplyBodyColor1(s32 arg0, s32 arg1);
void ApplyBodyColor2(s32 arg0, s32 arg1);
extern s32 g_ImageBlockBuffer;
/* Where asset 0x56 lands: g_ImageBlockBuffer advanced past the car texture
 * block just loaded. Its header words 1 and 2 are relocated into
 * g_AssetBlockPtr / g_AssetSubBlockPtr and word 0 is kept as-is. */
extern s32 g_AssetBlockPtr2;
void SelectCarModelSlot(s32);

void LoadUpgradedCarModel(s32 arg0) {
    u8 *ptr;
    GameCarModelAsset *asset;
    s32 offset;
    s32 assetId;
    u32 mode;

    if (g_AssetLoadState == 1) {
        offset = GetCarAssetIndex(arg0, g_CarTable[arg0].modelVariant + 1) << 1;
        mode = g_CarModelSlot;
        ptr = g_CarModelBuffer;
        assetId = offset + 0xA;

        if (mode == 0) {
            ptr += 0x20000;
        }

        if (LoadAsset(assetId, ptr) != 0) {
            asset = (GameCarModelAsset *)ptr;
            SetCarModelSlot(ptr, g_CarModelSlot < 1);

            asset->modelDataOffset = (s32)ptr + asset->modelDataOffset;
            RegisterModelBank((s32 *)asset->modelDataOffset, g_CarModelSlot < 1);

            asset->imageDataOffset = (s32)ptr + asset->imageDataOffset;
            SetCarImageSlot((void *)asset->imageDataOffset, g_CarModelSlot < 1);

            if (g_PlayerCarIndex < 10) {
                ApplyBodyColor1(g_CarTable[arg0].paintColor1, asset->imageDataOffset);
                ApplyBodyColor2(g_CarTable[arg0].paintColor2, asset->imageDataOffset);
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
    s32 ptr;
    s32 offset;

    if (g_AssetLoadState == 1) {
        if (LoadAsset(9, g_AssetBase) != 0) {
            RegisterModelBank((s32 *)(g_AssetBase + 4), 0);
            SelectModelBank(0);

            ptr = (s32)g_AssetBase;
            offset = *(s32 *)ptr;
            g_AssetLoadState = 0;
            g_ImageBlockBuffer = ptr + offset;
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
            register s32 base asm("$2");

            scaled = index << 1;
            scaled += index;
            base = g_GrandPrixClass;
            scaled <<= 1;
            base += 0x4A;
            kind = scaled + base;
        }

        result = LoadAsset((s32)kind, (void *)g_ImageBlockBuffer);
        if (result != 0) {
            g_AssetLoadState = 2;
            g_AssetBlockPtr2 = result + g_ImageBlockBuffer;
        }
        break;
    case 2:
        if (LoadAsset(0x56, (void *)g_AssetBlockPtr2) != 0) {
            s32 ptr = g_AssetBlockPtr2;
            register s32 first asm("$2");
            register s32 second asm("$3");
            s32 third;

            first = *(s32 *)(ptr + 4);
            second = *(s32 *)(ptr + 8);
            first = ptr + first;
            second = ptr + second;
            g_AssetBlockPtr = (u8 *)first;
            g_AssetSubBlockPtr = (u8 *)second;
            third = *(s32 *)ptr;
            g_AssetLoadState = 0;
            g_SharedAssetWord0 = third;
        }
        break;
    }
}

void RelocateCarModel(void) {
    u32 temp;
    u32 *dst;
    u32 *src;
    u32 count;

    temp = g_CarModelAsset;
    if (temp != 0) {
        src = (u32 *)temp;
    } else {
        src = (u32 *)temp;
    }
    count = src[6];
    temp = (u32)g_AssetBase;
    if (count != 0) {
        dst = (u32 *)temp;
    } else {
        dst = (u32 *)temp;
    }
    count = count + 0x28;
    temp = count + (u32)dst;
    count >>= 2;
    g_AssetLoadCursor = (u8 *)temp;

    while (count != 0) {
        *dst = *src;
        src++;
        count--;
        dst++;
    }

    SetCarModelSlot(g_AssetBase, 0);
    temp = *(s32 *)(g_CarModelAsset + 0x20);
    UnrelocateModelBank((s32 *)(g_AssetBase + 0x28), temp);
    SelectCarModelSlot(0);
    *(u32 *)(g_CarModelAsset + 0x20) = (u32)(g_AssetBase + 0x28);
    RegisterModelBank((s32 *)(g_AssetBase + 0x28), 0);
}
