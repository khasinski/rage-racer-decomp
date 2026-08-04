/*
 * The disc asset loader: open RAGE.BIN, read a numbered asset into the load
 * buffer, and hand out its sub-blocks. SetTrackCameraTable looks like track
 * code from its name but is part of this: it installs sub-block 0 of the
 * loaded .2ND track pack as the CamRow base (D_8019C9A8), and its only callers
 * are load_track_data_assets.c and race_assets.c. That name is why this unit
 * sat in track/ until 2026-08-03.
 */
#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "psyq/cd.h"

void CdReadBreak(void) asm("func_80027634");
s32 func_8006A534(s32 arg0, s32 arg1);
long CdControl(long com, void *param, long result) asm("func_8006A5A4");
extern char g_MsgNowLoading[] asm("D_80010ADC");
extern char g_MsgReadBytes[] asm("D_80010AFC");
extern char g_MsgFileReadError[] asm("D_80010B08");
s32 CdRead(s32 arg0, void *arg1, s32 arg2) asm("func_80027688");
long CdReadSync(long arg0, long arg1) asm("func_80027790");
void func_8006A6DC(s32 arg0, void *arg1);
s32 func_80017C78(s32 arg0, s32 arg1);
extern char g_PathRageBin[] asm("D_80010B38");
extern char g_MsgFileNotFound[] asm("D_80010B44");
extern char g_MsgReadSectors[] asm("D_80010B58");
extern char g_MsgNowSearching[] asm("D_80010B68");
extern char g_PathRageStr[] asm("D_80010B80");
extern char g_MsgSearchOk[] asm("D_80010B8C");
void UploadImageAsset(void *arg0) asm("func_8001A3C0");

void SetTrackCameraTable(u32 value);
void SetTrackCameraTable(u32 value) {
    *(u32 *)0x8019C9A8 = value;
}

void ResetAssetLoader(void) asm("func_80017BE4");
void ResetAssetLoader(void) {
    if (g_CdLoadPhase == 4) {
        CdReadBreak();
    }

    g_CdLoadPhase = 0;
    g_AssetLoadState = 0;
    g_MainState = 0;
}

s32 EnableCdAudioMode(void) asm("func_80017C2C");
s32 EnableCdAudioMode(void) {
    u8 value;

    if (func_8006A534(1, 0) == 0) {
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
        if (func_8006A534(1, 0) != 0) {
            g_CdLoadPhase = 1;
        }
        return 0;

    case 1:
        func_8006A6DC(2, &g_AssetCdEntries[assetIndex]);
        g_CdLoadPhase = 2;
        return 0;

    case 2:
        if (func_8006A534(1, 0) != 0) {
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

void LoadDiscArchiveIndex(void) asm("func_80017E8C");
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

void InitAssetSystem(void) asm("func_80018038");
void InitAssetSystem(void) {
    void *ptr;

    LoadDiscArchiveIndex();
    ptr = &g_LoadBuffer;
    LoadAssetBlocking(0, (s32)ptr);
    UploadImageAsset(ptr);
}

s32 RequestBootAssets(void) asm("func_80018078");
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
