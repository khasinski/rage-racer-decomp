/*
 * The disc asset loader: open RAGE.BIN, read a numbered asset into the load
 * buffer, and hand out its sub-blocks. SetTrackCameraTable looks like track
 * code from its name but is part of this: it installs sub-block 0 of the
 * loaded .2ND track pack as the CamRow base (g_CamRow), and its only callers
 * are load_track_data_assets.c and race_assets.c. That name is why this unit
 * sat in track/ until 2026-08-03.
 */
#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "psyq/cd.h"

void CdReadBreak(void);
long CdControl(long com, void *param, long result);
extern char g_MsgNowLoading[];
extern char g_MsgReadBytes[];
extern char g_MsgFileReadError[];
s32 CdRead(s32 arg0, void *arg1, s32 arg2);
long CdReadSync(long arg0, long arg1);
s32 func_80017C78(s32 arg0, s32 arg1);
extern char g_PathRageBin[];
extern char g_MsgFileNotFound[];
extern char g_MsgReadSectors[];
extern char g_MsgNowSearching[];
extern char g_PathRageStr[];
extern char g_MsgSearchOk[];
void UploadImageAsset(void *arg0);

void SetTrackCameraTable(u32 value);
void SetTrackCameraTable(u32 value) {
    *(u32 *)0x8019C9A8 = value;
}

void ResetAssetLoader(void);
void ResetAssetLoader(void) {
    if (g_CdLoadPhase == 4) {
        CdReadBreak();
    }

    g_CdLoadPhase = 0;
    g_AssetLoadState = 0;
    g_MainState = 0;
}

s32 EnableCdAudioMode(void);
s32 EnableCdAudioMode(void) {
    u8 value;

    if (CdSync(1, 0) == 0) {
        return 0;
    }

    value = 7;
    return CdControl(0xE, &value, 0);
}

s32 LoadAsset(s32 assetIndex, void *dst) {
    s32 result;
    s32 size;

    switch (g_CdLoadPhase) {
    case 0:
        DebugPrintf(g_MsgNowLoading, g_AssetPaths[assetIndex], dst);
        if (CdSync(1, 0) != 0) {
            g_CdLoadPhase = 1;
        }
        return 0;

    case 1:
        CdControlF(2, &g_AssetCdEntries[assetIndex]);
        g_CdLoadPhase = 2;
        return 0;

    case 2:
        if (CdSync(1, 0) != 0) {
            g_CdLoadPhase = 3;
        }
        return 0;

    case 3:
        if (CdRead((g_AssetCdEntries[assetIndex].size + 0x7FF) >> 11, dst, 0x80) != 0) {
            g_CdLoadPhase = 4;
        }
        return 0;

    case 4:
        result = CdReadSync(1, 0);
        if (result == 0) {
            g_CdLoadPhase = 5;
            return 0;
        }
        if (result == -1) {
            g_CdLoadPhase = 6;
        }
        return 0;

    case 5:
        size = (g_AssetCdEntries[assetIndex].size >> 2) << 2;
        DebugPrintf(g_MsgReadBytes, size);
        g_CdLoadPhase = 0;
        return size;

    case 6:
        DebugPrintf(g_MsgFileReadError, g_AssetPaths[assetIndex], dst);
        g_CdLoadPhase = 0;
        break;
    }

    return 0;
}

void LoadAssetBlocking(s32 arg0, s32 arg1);
void LoadAssetBlocking(s32 arg0, s32 arg1) {
    while (func_80017C78(arg0, arg1) == 0) {
    }
}

void LoadDiscArchiveIndex(void);
void LoadDiscArchiveIndex(void) {
    struct {
        CdlLOC file;
        u8 pad[20];
    } stack;
    s32 one;
    s32 base;
    s32 i;
    s32 status;
    s32 *src;
    GameCdLoadEntry *dst;
    GameCdLoadEntry *smallSrc;

    DebugPrintf(g_MsgNowLoading, g_PathRageBin, g_LoadBuffer);
    if (DsSearchFile(&stack.file, g_PathRageBin) == 0) {
        DebugPrintf(g_MsgFileNotFound, g_PathRageBin);
    }

    one = 1;
    do {
        CdControl(2, &stack.file, 0);
        CdRead(one, g_LoadBuffer, 0x80);
        do {
            status = CdReadSync(1, 0);
        } while (status > 0);
    } while (status != 0);

    DebugPrintf(g_MsgReadSectors, one);
    base = CdPosToInt_Local(&stack.file);
    src = g_LoadBuffer;
    dst = g_AssetCdEntries;
    for (i = 0; i < 135; i++) {
        CdIntToPos(base + *src, (CdlLOC *)dst);
        dst->size = src[1];
        src += 2;
        dst++;
    }

    DebugPrintf(g_MsgNowSearching, g_PathRageStr);
    if (DsSearchFile(&stack.file, g_PathRageStr) == 0) {
        DebugPrintf(g_MsgFileNotFound, g_PathRageStr);
    } else {
        DebugPrintf(g_MsgSearchOk);
    }

    base = CdPosToInt_Local(&stack.file);
    smallSrc = g_StreamCdEntries;
    for (i = 0; i < 11; i++) {
        CdIntToPos(base + smallSrc->position, (CdlLOC *)smallSrc);
        smallSrc++;
    }
}

void InitAssetSystem(void);
void InitAssetSystem(void) {
    void *ptr;

    LoadDiscArchiveIndex();
    ptr = &g_LoadBuffer;
    LoadAssetBlocking(0, (s32)ptr);
    UploadImageAsset(ptr);
}

s32 RequestBootAssets(void);
s32 RequestBootAssets(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 1) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 1;
    g_AssetLoadState = 1;
    return 1;
}
