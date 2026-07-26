#include "common.h"
#include "game/asset.h"
#include "game/race.h"
#include "game/state.h"

extern GameSceneAssetHeader *g_AssetLoadCursor asm("D_8019CAFC");

s32 func_80017C78(s32 assetIndex, void *dst);
s32 GameEnableCdAudioMode(void) asm("func_80017C2C");
void GameSetTrackCameraTable(void *arg0) asm("func_80017BD4");
void func_8004553C(void *arg0);
void func_8004550C(void *arg0);
void GameRegisterModelBank(void *arg0, s32 arg1) asm("func_80017948");
void func_8002A6B0(void *arg0);
void GameRegisterCourseModels(void *arg0) asm("func_80017A6C");
void func_80017AD0(void *arg0);
void func_8004121C(void *arg0);
void func_80034E04(void *arg0);
void func_80043AC8(void *arg0, s32 arg1);

void GameLoadTrackDataAssets(void) asm("func_8001989C");
void GameLoadTrackDataAssets(void) {
    register GameSceneAssetHeader *header asm("$4");
    register void *dst asm("$5");
    register s32 offset asm("$2");

    switch (g_AssetLoadState) {
    case 1:
        dst = g_AssetLoadCursor;
        offset = g_CourseIndex;
        __asm__ volatile("" : "=r"(offset) : "0"(offset));
        header = (GameSceneAssetHeader *)g_GrandPrixClass;
        if (func_80017C78(((s32)header << 3) + (offset << 1) + 0x58, dst) != 0) {
            header = g_AssetLoadCursor;
            offset = header->offsets[0];
            g_AssetBlockPtr = (u8 *)header + offset;
            GameSetTrackCameraTable(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[1];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_8004553C(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[2];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_8004550C(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[3];
            g_AssetBlockPtr = (u8 *)header + offset;
            GameRegisterModelBank(g_AssetBlockPtr, 1);

            header = g_AssetLoadCursor;
            offset = header->offsets[4];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_8002A6B0(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[5];
            g_AssetBlockPtr = (u8 *)header + offset;
            GameRegisterCourseModels(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[6];
            g_AssetBlockPtr = (u8 *)header + offset;
            GameRegisterModelBank(g_AssetBlockPtr, 2);

            header = g_AssetLoadCursor;
            offset = header->offsets[7];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80017AD0(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[8];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_8004121C(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[9];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80034E04(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[10];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80043AC8(g_AssetBlockPtr, 0);

            g_AssetLoadState = 2;
        }
        break;
    case 2:
        if (GameEnableCdAudioMode() != 0) {
            g_AssetLoadState = 0;
        }
        break;
    }
}

extern s32 *D_801E8A90;
extern s32 D_8019CA1C;
extern s32 D_8019C708;

void GameBeginFmv(void) asm("func_8001E6B4");

void GameBeginIntroFmv(void) asm("func_80019AF0");
void GameBeginIntroFmv(void) {
    s32 *ptr;
    s32 value;

    GameBeginFmv();

    ptr = (s32 *)&g_StreamCdEntries[0].size;
    value = *ptr;
    D_801E8A90 = ptr - 1;
    D_8019CA1C = value;
    D_8019C708 = value * 2;
}

extern s32 *D_801E8A90;
extern s32 D_8019CA1C;
extern s32 D_8019C708;

void GameBeginFmv(void) asm("func_8001E6B4");

void GameBeginClassFmv(void) asm("func_80019B3C");
void GameBeginClassFmv(void) {
    s32 *base;
    s32 offset;
    s32 index;
    s32 value;

    GameBeginFmv();

    if (g_SeriesSelection != 0) {
        index = 5;
    } else {
        index = 1;
    }

    base = (s32 *)g_StreamCdEntries;
    index += g_GrandPrixClass;
    offset = index << 3;
    value = *(s32 *)((s32)(s32 *)&g_StreamCdEntries[0].size + offset);
    D_801E8A90 = (s32 *)((s32)base + offset);
    D_8019CA1C = value;
    D_8019C708 = value * 2;
}

extern s32 *D_801E8A90;
extern s32 D_8019CA1C;
extern s32 D_8019C708;

void GameBeginFmv(void) asm("func_8001E6B4");

void GameBeginEndingFmv(void) asm("func_80019BB8");
void GameBeginEndingFmv(void) {
    s32 *ptr;
    s32 value;

    GameBeginFmv();

    ptr = (s32 *)&g_StreamCdEntries[10].size;
    value = *ptr;
    D_801E8A90 = ptr - 1;
    D_8019CA1C = value;
    D_8019C708 = value * 4;
}

extern s32 D_8009AEFC;

void GameLoadBootAssets(void) asm("func_800180CC");
void GameLoadSaveScreenAssets(void) asm("func_80018344");
void GameLoadSelectBgmAssets(void) asm("func_80018484");
void GameLoadCarSelectAssets(void) asm("func_80018588");
void GameLoadCarModel(s32) asm("func_800188B8");
void GameLoadUpgradedCarModel(s32) asm("func_80018A70");
void GameLoadOptionScreenAssets(void) asm("func_80018C0C");
void GameLoadRoundAssets(void) asm("func_80018DF8");
void GameLoadRaceAssets(void) asm("func_8001901C");
void GameLoadGrandPrixScreen(void) asm("func_800195F4");
void GameLoadCourseAssets(void) asm("func_800196B4");
void GameLoadTrackDataAssets(void) asm("func_8001989C");

void GameServiceAssetLoad(void) asm("func_80019C04");
void GameServiceAssetLoad(void) {
    if (g_AssetLoadState != 0) {
        switch (g_MainState) {
        case 1:
            GameLoadBootAssets();
            break;
        case 2:
            GameLoadSaveScreenAssets();
            break;
        case 3:
            GameLoadSelectBgmAssets();
            break;
        case 4:
            GameLoadCarSelectAssets();
            break;
        case 5:
            GameLoadCarModel(D_8009AEFC);
            break;
        case 6:
            GameLoadUpgradedCarModel(D_8009AEFC);
            break;
        case 7:
            GameLoadOptionScreenAssets();
            break;
        case 8:
            GameLoadRoundAssets();
            break;
        case 9:
            GameLoadRaceAssets();
            break;
        case 10:
            GameLoadGrandPrixScreen();
            break;
        case 11:
            GameLoadCourseAssets();
            break;
        case 12:
            GameLoadTrackDataAssets();
            break;
        }
    }
}
