#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"

/*
 * Every `X + *(s32 *)(X + 4*k)` below is sub-block k of the loaded asset
 * pack's GameSceneAssetHeader offset table (game/asset.h).
 */
#define ASSET_SUB(base, k) ((base) + ((GameSceneAssetHeader *)(base))->offsets[k])

s32 GameRequestRaceStart(void) asm("func_80019580");

void GameLoadGrandPrixScreen(void) asm("func_800195F4");

void GameLoadCourseAssets(void) asm("func_800196B4");

s32 GameRequestTrackLoad(void) asm("func_8001965C");

extern u8 *D_8019CAFC;
extern u8 *D_801E8AB0;
extern u8 *D_801E42D0;
extern s32 D_801E4D70;
extern u8 *D_8019C754;
void func_8005B768(s32 arg0, void *arg1, void *arg2, void *arg3);
s32 func_8005B89C(void);
s32 GameGetCarAssetIndex(s32 model, s32 grade) asm("func_80017848");
s32 func_80017C78(s32 arg0, void *arg1);
void func_80034DF4(void);
void func_8001A3C0(void *arg0);
void func_8001A2E0(void *arg0);
void func_8001A40C(void *arg0);
void func_80019EBC(void);
void func_80017BD4(void *arg0);
void func_8004553C(void *arg0);
void func_8004550C(void *arg0);
void func_80017948(void *arg0, s32 arg1);
void func_8002A6B0(void *arg0);
void func_80017A6C(void *arg0);
void func_80017AD0(void *arg0);
void func_8004121C(void *arg0);
void func_80034E04(void *arg0);
void func_80043AC8(void *arg0, s32 arg1);
s32 func_80017C2C(void);
void func_80042C94(void);
extern s32 D_801E4B30;
extern s32 D_8019C904;

s32 func_80018FC4(void) {
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

void func_8001901C(void) {
    switch (g_AssetLoadState) {
    case 1: {
        s32 *src = (s32 *)g_AssetBlockPtr;
        s32 raw = D_801E4D70;
        s32 *dst = (s32 *)D_8019CAFC;
        s32 n = raw / 4;
        while (n != 0) {
            *dst = *src;
            src++;
            n--;
            dst++;
        }
        func_8005B768(2, D_8019CAFC, D_801E8AB0, 0);
        g_AssetLoadState = 2;
        D_8019CAFC = D_8019CAFC + D_801E4D70;
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
        if (func_80017C78((sz << 1) + 11, D_8019CAFC) != 0) {
            register u8 *base_a0 asm("$4");
            register u8 *base_a3 asm("$7");
            register u8 *p1 asm("$5");
            register u8 *p2 asm("$6");
            base_a0 = D_8019CAFC;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 0);
            func_80034DF4();
            base_a3 = D_8019CAFC;
            p1 = ASSET_SUB(base_a3, 1);
            p2 = ASSET_SUB(base_a3, 3);
            base_a3 = ASSET_SUB(base_a3, 2);
            g_AssetBlockPtr = p1;
            D_8019C754 = base_a3;
            D_801E8AB0 = p2;
            func_8005B768(3, p1, p2, base_a3);
            base_a0 = D_8019CAFC;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 4);
            func_8001A3C0(g_AssetBlockPtr);
            g_AssetLoadState = 4;
            D_8019CAFC = D_801E8AB0;
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
        p = D_8019CAFC;
        scaled = g_CourseIndex << 1;
        base_off = (g_GrandPrixClass << 3) + 0x57;
        if (func_80017C78(scaled + base_off, p) != 0) {
            register u8 *base_a0 asm("$4");
            u8 *base;
            s32 off0, off1;
            base_a0 = D_8019CAFC;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 0);
            func_8001A3C0(g_AssetBlockPtr);
            base_a0 = D_8019CAFC;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 1);
            func_8001A3C0(g_AssetBlockPtr);
            base_a0 = D_8019CAFC;
            g_AssetBlockPtr = ASSET_SUB(base_a0, 2);
            func_8001A2E0(g_AssetBlockPtr);
            base = D_8019CAFC;
            off0 = ((GameSceneAssetHeader *)base)->offsets[3];
            off1 = ((GameSceneAssetHeader *)base)->offsets[4];
            g_AssetBlockPtr = base + off0;
            D_801E8AB0 = base + off1;
            func_8001A3C0(g_AssetBlockPtr);
            func_8001A40C(D_8019CAFC);
            D_801E42D0 = D_8019CAFC;
            func_8001A3C0(D_801E8AB0);
            func_80019EBC();
            g_AssetLoadState = 6;
            D_8019CAFC = D_8019CAFC + 0x38000;
        }
        break;
    }
    case 6: {
        register u8 *p asm("$5");
        register s32 scaled asm("$3");
        register s32 result asm("$2");
        p = D_8019CAFC;
        scaled = g_CourseIndex << 1;
        result = (g_GrandPrixClass << 3) + scaled;
        if (func_80017C78(result + 0x58, p) != 0) {
            register u8 *base_a0 asm("$4");
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 0); func_80017BD4(g_AssetBlockPtr);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 1); func_8004553C(g_AssetBlockPtr);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 2); func_8004550C(g_AssetBlockPtr);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 3); func_80017948(g_AssetBlockPtr, 1);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 4); func_8002A6B0(g_AssetBlockPtr);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 5); func_80017A6C(g_AssetBlockPtr);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 6); func_80017948(g_AssetBlockPtr, 2);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 7); func_80017AD0(g_AssetBlockPtr);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 8); func_8004121C(g_AssetBlockPtr);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 9); func_80034E04(g_AssetBlockPtr);
            base_a0 = D_8019CAFC; g_AssetBlockPtr = ASSET_SUB(base_a0, 10); func_80043AC8(g_AssetBlockPtr, 1);
            g_AssetLoadState = 7;
        }
        break;
    }
    case 7:
        if (func_80017C2C() != 0) {
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

    func_80042C94();
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
        value = func_80017C78((s32)(offset + base), (void *)D_801E4B30);
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

        value = func_80017C78((s32)(left + right), (void *)D_8019C904);
        if (value != 0) {
            g_AssetLoadState = 0;
            D_801E4B30 = value + D_8019C904;
        }
    }
}
