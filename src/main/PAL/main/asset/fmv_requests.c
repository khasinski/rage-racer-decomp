#include "common.h"
#include "game/asset.h"
#include "game/race.h"
#include "game/state.h"
s32 LoadAsset(s32 assetIndex, void *dst) asm("func_80017C78");

extern GameSceneAssetHeader *g_AssetLoadCursor asm("D_8019CAFC");

s32 EnableCdAudioMode(void) asm("func_80017C2C");
void SetTrackCameraTable(void *arg0);
void SetEnvPaletteTable(void *arg0);
void SetEnvironmentScript(void *arg0);
void RegisterModelBank(void *arg0, s32 arg1) asm("func_80017948");
void InstallTrackPoints(void *arg0) asm("func_8002A6B0");
void RegisterCourseModels(void *arg0) asm("func_80017A6C");
void InstallTerrainCellData(void *arg0) asm("func_80017AD0");
void SetCourseObjects(void *arg0);
void InstallTrackEventData(void *arg0);
void SelectTrackCameraTable(void *arg0, s32 arg1);

void LoadTrackDataAssets(void) asm("func_8001989C");
void LoadTrackDataAssets(void) {
    GameSceneAssetHeader *header;
    register void *dst asm("$5");
    s32 offset;

    switch (g_AssetLoadState) {
    case 1:
        dst = g_AssetLoadCursor;
        offset = g_CourseIndex;
        __asm__ volatile("" : "=r"(offset) : "0"(offset));
        header = (GameSceneAssetHeader *)g_GrandPrixClass;
        if (LoadAsset(((s32)header * 8) + (offset * 2) + 0x58, dst) != 0) {
            header = g_AssetLoadCursor;
            offset = header->offsets[0];
            g_AssetBlockPtr = (u8 *)header + offset;
            SetTrackCameraTable(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[1];
            g_AssetBlockPtr = (u8 *)header + offset;
            SetEnvPaletteTable(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[2];
            g_AssetBlockPtr = (u8 *)header + offset;
            SetEnvironmentScript(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[3];
            g_AssetBlockPtr = (u8 *)header + offset;
            RegisterModelBank(g_AssetBlockPtr, 1);

            header = g_AssetLoadCursor;
            offset = header->offsets[4];
            g_AssetBlockPtr = (u8 *)header + offset;
            InstallTrackPoints(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[5];
            g_AssetBlockPtr = (u8 *)header + offset;
            RegisterCourseModels(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[6];
            g_AssetBlockPtr = (u8 *)header + offset;
            RegisterModelBank(g_AssetBlockPtr, 2);

            header = g_AssetLoadCursor;
            offset = header->offsets[7];
            g_AssetBlockPtr = (u8 *)header + offset;
            InstallTerrainCellData(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[8];
            g_AssetBlockPtr = (u8 *)header + offset;
            SetCourseObjects(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[9];
            g_AssetBlockPtr = (u8 *)header + offset;
            InstallTrackEventData(g_AssetBlockPtr);

            header = g_AssetLoadCursor;
            offset = header->offsets[10];
            g_AssetBlockPtr = (u8 *)header + offset;
            SelectTrackCameraTable(g_AssetBlockPtr, 0);

            g_AssetLoadState = 2;
        }
        break;
    case 2:
        if (EnableCdAudioMode() != 0) {
            g_AssetLoadState = 0;
        }
        break;
    }
}

extern s32 *g_StreamLoc asm("D_801E8A90");
extern s32 g_StreamSectorCount asm("D_8019CA1C");
extern s32 D_8019C708;

void BeginFmv(void);

void BeginIntroFmv(void);
void BeginIntroFmv(void) {
    s32 *ptr;
    s32 value;

    BeginFmv();

    ptr = (s32 *)&g_StreamCdEntries[0].size;
    value = *ptr;
    g_StreamLoc = ptr - 1;
    g_StreamSectorCount = value;
    D_8019C708 = value * 2;
}

extern s32 *g_StreamLoc;
extern s32 g_StreamSectorCount;
extern s32 D_8019C708;

void BeginFmv(void);

void BeginClassFmv(void);
void BeginClassFmv(void) {
    s32 *base;
    s32 offset;
    s32 index;
    s32 value;

    BeginFmv();

    if (g_SeriesSelection != 0) {
        index = 5;
    } else {
        index = 1;
    }

    base = (s32 *)g_StreamCdEntries;
    index += g_GrandPrixClass;
    offset = index * 8;
    value = g_StreamCdEntries[index].size;
    g_StreamLoc = (s32 *)((u8 *)base + offset);
    g_StreamSectorCount = value;
    D_8019C708 = value * 2;
}

extern s32 *g_StreamLoc;
extern s32 g_StreamSectorCount;
extern s32 D_8019C708;

void BeginFmv(void);

void BeginEndingFmv(void);
void BeginEndingFmv(void) {
    s32 *ptr;
    s32 value;

    BeginFmv();

    ptr = (s32 *)&g_StreamCdEntries[10].size;
    value = *ptr;
    g_StreamLoc = ptr - 1;
    g_StreamSectorCount = value;
    D_8019C708 = value * 4;
}

extern s32 g_PendingCarModelIndex asm("D_8009AEFC");

void LoadBootAssets(void) asm("func_800180CC");
void LoadSaveScreenAssets(void);
void LoadSelectBgmAssets(void) asm("func_80018484");
void LoadCarSelectAssets(void) asm("func_80018588");
void LoadCarModel(s32);
void LoadUpgradedCarModel(s32);
void LoadOptionScreenAssets(void);
void LoadRoundAssets(void);
void LoadRaceAssets(void) asm("func_8001901C");
void LoadGrandPrixScreen(void);
void LoadCourseAssets(void);
void LoadTrackDataAssets(void) asm("func_8001989C");

void ServiceAssetLoad(void) asm("func_80019C04");
void ServiceAssetLoad(void) {
    if (g_AssetLoadState != 0) {
        switch (g_MainState) {
        case 1:
            LoadBootAssets();
            break;
        case 2:
            LoadSaveScreenAssets();
            break;
        case 3:
            LoadSelectBgmAssets();
            break;
        case 4:
            LoadCarSelectAssets();
            break;
        case 5:
            LoadCarModel(g_PendingCarModelIndex);
            break;
        case 6:
            LoadUpgradedCarModel(g_PendingCarModelIndex);
            break;
        case 7:
            LoadOptionScreenAssets();
            break;
        case 8:
            LoadRoundAssets();
            break;
        case 9:
            LoadRaceAssets();
            break;
        case 10:
            LoadGrandPrixScreen();
            break;
        case 11:
            LoadCourseAssets();
            break;
        case 12:
            LoadTrackDataAssets();
            break;
        }
    }
}
