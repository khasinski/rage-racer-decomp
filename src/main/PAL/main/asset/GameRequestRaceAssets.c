#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"
#include "game/cd.h"

/*
 * Every `X + *(s32 *)(X + 4*k)` below is sub-block k of the loaded asset
 * pack's GameSceneAssetHeader offset table (game/asset.h).
 */
#define ASSET_SUB(base, k) ((base) + ((GameSceneAssetHeader *)(base))->offsets[k])

s32 GameRequestRaceStart(void) asm("func_80019580");

void GameLoadGrandPrixScreen(void) asm("func_800195F4");

void GameLoadCourseAssets(void) asm("func_800196B4");

s32 GameRequestTrackLoad(void) asm("func_8001965C");

extern u8 *g_AssetLoadCursor asm("D_8019CAFC");
extern u8 *g_AssetSubBlockPtr asm("D_801E8AB0");
extern u8 *g_TrackTextureShadow asm("D_801E42D0");
extern s32 g_SharedAssetWord0 asm("D_801E4D70");
extern u8 *g_AssetBlockPtr2 asm("D_8019C754");
void func_8005B768(s32 arg0, void *arg1, void *arg2, void *arg3);
s32 func_8005B89C(void);
s32 GameGetCarAssetIndex(s32 model, s32 grade) asm("func_80017848");
s32 func_80017C78(s32 arg0, void *arg1);
void func_80034DF4(void);
void GameUploadImageAsset(void *arg0) asm("func_8001A3C0");
void func_8001A2E0(void *arg0);
void func_8001A40C(void *arg0);
void GameResetTrackTextureSwap(void) asm("func_80019EBC");
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
s32 GameEnableCdAudioMode(void) asm("func_80017C2C");
extern s32 g_ImageBlockBuffer asm("D_801E4B30");
extern s32 g_AssetBase asm("D_8019C904");

s32 GameRequestRaceAssets(void) asm("func_80018FC4");
s32 GameRequestRaceAssets(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 9) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 9;
    g_AssetLoadState = 1;
    return 1;
}

void GameLoadRaceAssets(void) asm("func_8001901C");
void GameLoadRaceAssets(void) {
    switch (g_AssetLoadState) {
    case 1: {
        s32 *src = (s32 *)g_AssetBlockPtr;
        s32 raw = g_SharedAssetWord0;
        s32 *dst = (s32 *)g_AssetLoadCursor;
        s32 n = raw / 4;
        while (n != 0) {
            *dst = *src;
            src++;
            n--;
            dst++;
        }
        func_8005B768(2, g_AssetLoadCursor, g_AssetSubBlockPtr, 0);
        g_AssetLoadState = 2;
        g_AssetLoadCursor = g_AssetLoadCursor + g_SharedAssetWord0;
        break;
    }
    case 2:
        if ((func_8005B89C() << 16) != 0) {
            g_AssetLoadState = 3;
        }
        break;
    case 3: {
        s32 idx = g_PlayerCarIndex;
        s32 sz = GameGetCarAssetIndex(idx, g_CarTable[idx].modelVariant);
        if (func_80017C78((sz << 1) + 11, g_AssetLoadCursor) != 0) {
            register u8 *base_a0 asm("$4");
            register u8 *base_a3 asm("$7");
            register u8 *p1 asm("$5");
            register u8 *p2 asm("$6");
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 0);
            func_80034DF4();
            base_a3 = g_AssetLoadCursor;
            p1 = ASSET_SUB(base_a3, 1);
            p2 = ASSET_SUB(base_a3, 3);
            base_a3 = ASSET_SUB(base_a3, 2);
            g_AssetBlockPtr = p1;
            g_AssetBlockPtr2 = base_a3;
            g_AssetSubBlockPtr = p2;
            func_8005B768(3, p1, p2, base_a3);
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 4);
            GameUploadImageAsset(g_AssetBlockPtr);
            g_AssetLoadState = 4;
            g_AssetLoadCursor = g_AssetSubBlockPtr;
        }
        break;
    }
    case 4:
        if ((func_8005B89C() << 16) != 0) {
            g_AssetLoadState = 5;
        }
        break;
    case 5: {
        register u8 *p asm("$5");
        register s32 scaled asm("$2");
        register s32 base_off asm("$4");
        p = g_AssetLoadCursor;
        scaled = g_CourseIndex << 1;
        base_off = (g_GrandPrixClass << 3) + 0x57;
        if (func_80017C78(scaled + base_off, p) != 0) {
            register u8 *base_a0 asm("$4");
            u8 *base;
            s32 off0, off1;
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 0);
            GameUploadImageAsset(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 1);
            GameUploadImageAsset(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 2);
            func_8001A2E0(g_AssetBlockPtr);
            base = g_AssetLoadCursor;
            off0 = ((GameSceneAssetHeader *)base)->offsets[3];
            off1 = ((GameSceneAssetHeader *)base)->offsets[4];
            g_AssetBlockPtr = base + off0;
            g_AssetSubBlockPtr = base + off1;
            GameUploadImageAsset(g_AssetBlockPtr);
            func_8001A40C(g_AssetLoadCursor);
            g_TrackTextureShadow = g_AssetLoadCursor;
            GameUploadImageAsset(g_AssetSubBlockPtr);
            GameResetTrackTextureSwap();
            g_AssetLoadState = 6;
            g_AssetLoadCursor = g_AssetLoadCursor + 0x38000;
        }
        break;
    }
    case 6: {
        register u8 *p asm("$5");
        register s32 scaled asm("$3");
        register s32 result asm("$2");
        p = g_AssetLoadCursor;
        scaled = g_CourseIndex << 1;
        result = (g_GrandPrixClass << 3) + scaled;
        if (func_80017C78(result + 0x58, p) != 0) {
            register u8 *base_a0 asm("$4");
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 0); GameSetTrackCameraTable(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 1); func_8004553C(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 2); func_8004550C(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 3); GameRegisterModelBank(g_AssetBlockPtr, 1);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 4); func_8002A6B0(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 5); GameRegisterCourseModels(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 6); GameRegisterModelBank(g_AssetBlockPtr, 2);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 7); func_80017AD0(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 8); func_8004121C(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 9); func_80034E04(g_AssetBlockPtr);
            base_a0 = g_AssetLoadCursor; g_AssetBlockPtr = ASSET_SUB(base_a0, 10); func_80043AC8(g_AssetBlockPtr, 1);
            g_AssetLoadState = 7;
        }
        break;
    }
    case 7:
        if (GameEnableCdAudioMode() != 0) {
            g_AssetLoadState = 0;
        }
        break;
    }
}

s32 GameRequestRaceStart(void) {
    s32 state;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    state = 10;
    if (g_MainState == state) {
        g_MainState = 0;
        return 0;
    }

    GameResetCdAudioState();
    g_MainState = state;
    g_AssetLoadState = 1;
    return 1;
}

void GameLoadGrandPrixScreen(void) {
    s32 base;
    s32 offset;
    s32 value;

    if (g_AssetLoadState == 1) {
        offset = g_GrandPrixSeries * 6;
        base = g_GrandPrixClass + 0x4A;
        value = func_80017C78((s32)(offset + base), (void *)g_ImageBlockBuffer);
        if (value != 0) {
            g_AssetLoadState = 0;
        }
    }
}

s32 GameRequestTrackLoad(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 0xB) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 0xB;
    g_AssetLoadState = 1;
    return 1;
}

void GameLoadCourseAssets(void) {
    s32 value;

    if (g_AssetLoadState == 1) {
        s32 left = g_CourseIndex << 1;
        s32 right = (g_GrandPrixClass << 3) + 0x57;

        value = func_80017C78((s32)(left + right), (void *)g_AssetBase);
        if (value != 0) {
            g_AssetLoadState = 0;
            g_ImageBlockBuffer = value + g_AssetBase;
        }
    }
}
