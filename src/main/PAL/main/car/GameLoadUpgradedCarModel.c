#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/state.h"
#include "game/race.h"
#include "game/render.h"
#include "game/cd.h"

extern u8 *g_CarModelAsset asm("D_8009E698");

extern u32 g_CarModelSlot asm("D_8009E87C");
extern u8 *g_CarModelBuffer asm("D_801E4090");
s32 GameGetCarAssetIndex(s32 model, s32 grade) asm("func_80017848");
s32 func_80017C78(s32 arg0, void *arg1);
void GameSetCarModelSlot(void *arg0, s32 arg1) asm("func_80017B94");
void GameRegisterModelBank(void *arg0, s32 arg1) asm("func_80017948");
void GameSetCarImageSlot(void *arg0, s32 arg1) asm("func_80017B44");
void GameApplyBodyColor1(s32 arg0, s32 arg1) asm("func_8001D748");
void GameApplyBodyColor2(s32 arg0, s32 arg1) asm("func_8001D900");
extern s32 g_AssetBase asm("D_8019C904");
extern s32 g_ImageBlockBuffer asm("D_801E4B30");
s32 GameRandom15(void) asm("func_800632B0");
void GameResetAssetLoader(void) asm("func_80017BE4");
/* Where asset 0x56 lands: g_ImageBlockBuffer advanced past the car texture
 * block just loaded. Its header words 1 and 2 are relocated into
 * g_AssetBlockPtr / g_AssetSubBlockPtr and word 0 is kept as-is. */
extern s32 g_AssetBlockPtr2 asm("D_8019C754");
extern s32 g_SharedAssetWord0 asm("D_801E4D70");
extern s32 g_AssetSubBlockPtr asm("D_801E8AB0");
extern u32 g_AssetLoadCursor asm("D_8019CAFC");
void GameUnrelocateModelBank(void *, s32) asm("func_800179B4");
void GameSelectCarModelSlot(s32) asm("func_80017BAC");

void GameLoadUpgradedCarModel(s32 arg0) asm("func_80018A70");
void GameLoadUpgradedCarModel(s32 arg0) {
    u8 *ptr;
    GameCarModelAsset *asset;
    s32 offset;
    s32 assetId;
    u32 mode;

    if (g_AssetLoadState == 1) {
        offset = GameGetCarAssetIndex(arg0, g_CarTable[arg0].modelVariant + 1) << 1;
        mode = g_CarModelSlot;
        ptr = g_CarModelBuffer;
        assetId = offset + 0xA;

        if (mode == 0) {
            ptr += 0x20000;
        }

        if (func_80017C78(assetId, ptr) != 0) {
            asset = (GameCarModelAsset *)ptr;
            GameSetCarModelSlot(ptr, g_CarModelSlot < 1);

            asset->modelDataOffset = (s32)ptr + asset->modelDataOffset;
            GameRegisterModelBank((void *)asset->modelDataOffset, g_CarModelSlot < 1);

            asset->imageDataOffset = (s32)ptr + asset->imageDataOffset;
            GameSetCarImageSlot((void *)asset->imageDataOffset, g_CarModelSlot < 1);

            if (g_PlayerCarIndex < 10) {
                GameApplyBodyColor1(g_CarTable[arg0].paintColor1, asset->imageDataOffset);
                GameApplyBodyColor2(g_CarTable[arg0].paintColor2, asset->imageDataOffset);
            }

            g_AssetLoadState = 0;
        }
    }
}

s32 GameRequestOptionScreenAssets(void) asm("func_80018B98");
s32 GameRequestOptionScreenAssets(void) {
    s32 state;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    state = 7;
    if (g_MainState == state) {
        g_MainState = 0;
        return 0;
    }

    GameResetCdAudioState();
    g_MainState = state;
    g_AssetLoadState = 1;
    return 1;
}

void GameLoadOptionScreenAssets(void) asm("func_80018C0C");
void GameLoadOptionScreenAssets(void) {
    s32 ptr;
    s32 offset;

    if (g_AssetLoadState == 1) {
        if (func_80017C78(9, (void *)g_AssetBase) != 0) {
            GameRegisterModelBank((void *)(g_AssetBase + 4), 0);
            GameSelectModelBank(0);

            ptr = g_AssetBase;
            offset = *(s32 *)ptr;
            g_AssetLoadState = 0;
            g_ImageBlockBuffer = ptr + offset;
        }
    }
}

s32 GameRequestRoundAssets(void) asm("func_80018C88");
s32 GameRequestRoundAssets(void) {
    s32 value;

    if (g_AssetLoadState != 0) {
        GameResetAssetLoader();
    }

    if (g_GrandPrixMode == 0) {
        value = (GameRandom15() & 0xFFF) % (g_MaxClassReached[g_GrandPrixSeries] + 1);
        g_GrandPrixClass = value;
        if (((g_CourseIndex & 3) == 3) && (value < 2)) {
            g_GrandPrixClass = ((GameRandom15() & 0xFFF) % (g_MaxClassReached[g_GrandPrixSeries] - 1)) + 2;
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

void GameLoadRoundAssets(void) asm("func_80018DF8");
void GameLoadRoundAssets(void) {
    s32 state;
    s32 kind;
    s32 result;

    state = g_AssetLoadState;
    switch (state) {
    case 1:
        kind = 0x55;
        if (g_GrandPrixMode != 0) {
            /* These pins are load-bearing: removing any one changes .text. */
            register s32 index asm("$2") = g_GrandPrixSeries;
            s32 scaled;
            register s32 base asm("$2");

            __asm__ volatile("");
            scaled = index << 1;
            scaled += index;
            base = g_GrandPrixClass;
            scaled <<= 1;
            base += 0x4A;
            kind = scaled + base;
        }

        result = func_80017C78((s32)kind, (void *)g_ImageBlockBuffer);
        if (result != 0) {
            g_AssetLoadState = 2;
            g_AssetBlockPtr2 = result + g_ImageBlockBuffer;
        }
        break;
    case 2:
        if (func_80017C78(0x56, (void *)g_AssetBlockPtr2) != 0) {
            s32 ptr = g_AssetBlockPtr2;
            /* These pins are load-bearing: removing any one changes .text. */
            register s32 first asm("$2");
            register s32 second asm("$3");
            s32 third;

            first = *(s32 *)(ptr + 4);
            second = *(s32 *)(ptr + 8);
            first = ptr + first;
            second = ptr + second;
            g_AssetBlockPtr = (u8 *)first;
            g_AssetSubBlockPtr = second;
            third = *(s32 *)ptr;
            g_AssetLoadState = 0;
            g_SharedAssetWord0 = third;
        }
        break;
    }
}

void GameRelocateCarModel(void) asm("func_80018F08");
void GameRelocateCarModel(void) {
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
    temp = g_AssetBase;
    if (count != 0) {
        dst = (u32 *)temp;
    } else {
        dst = (u32 *)temp;
    }
    count = count + 0x28;
    temp = count + (u32)dst;
    count >>= 2;
    g_AssetLoadCursor = temp;

    while (count != 0) {
        *dst = *src;
        src++;
        count--;
        dst++;
    }

    GameSetCarModelSlot((void *)g_AssetBase, 0);
    temp = *(s32 *)(g_CarModelAsset + 0x20);
    GameUnrelocateModelBank((void *)(g_AssetBase + 0x28), temp);
    GameSelectCarModelSlot(0);
    *(u32 *)(g_CarModelAsset + 0x20) = g_AssetBase + 0x28;
    GameRegisterModelBank((void *)(g_AssetBase + 0x28), 0);
}
