#include "common.h"
#include "game/asset.h"
#include "game/race.h"
#include "game/state.h"

extern GameSceneAssetHeader *g_AssetLoadCursor asm("D_8019CAFC");

s32 func_80017C78(s32 assetIndex, void *dst);
s32 func_80017C2C(void);
void func_80017BD4(void *arg0);
void func_8004553C(void *arg0);
void func_8004550C(void *arg0);
void GameRegisterModelBank(void *arg0, s32 arg1) asm("func_80017948");
void func_8002A6B0(void *arg0);
void func_80017A6C(void *arg0);
void func_80017AD0(void *arg0);
void func_8004121C(void *arg0);
void func_80034E04(void *arg0);
void func_80043AC8(void *arg0, s32 arg1);

void func_8001989C(void) {
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
            func_80017BD4(g_AssetBlockPtr);

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
            func_80017A6C(g_AssetBlockPtr);

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
        if (func_80017C2C() != 0) {
            g_AssetLoadState = 0;
        }
        break;
    }
}

extern s32 *D_801E8A90;
extern s32 D_8019CA1C;
extern s32 D_8019C708;

void func_8001E6B4(void);

void func_80019AF0(void) {
    s32 *ptr;
    s32 value;

    func_8001E6B4();

    ptr = (s32 *)&g_StreamCdEntries[0].size;
    value = *ptr;
    D_801E8A90 = ptr - 1;
    D_8019CA1C = value;
    D_8019C708 = value * 2;
}

extern s32 *D_801E8A90;
extern s32 D_8019CA1C;
extern s32 D_8019C708;

void func_8001E6B4(void);

void func_80019B3C(void) {
    s32 *base;
    s32 offset;
    s32 index;
    s32 value;

    func_8001E6B4();

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

void func_8001E6B4(void);

void func_80019BB8(void) {
    s32 *ptr;
    s32 value;

    func_8001E6B4();

    ptr = (s32 *)&g_StreamCdEntries[10].size;
    value = *ptr;
    D_801E8A90 = ptr - 1;
    D_8019CA1C = value;
    D_8019C708 = value * 4;
}

extern s32 D_8009AEFC;

void func_800180CC(void);
void func_80018344(void);
void func_80018484(void);
void func_80018588(void);
void func_800188B8(s32);
void func_80018A70(s32);
void func_80018C0C(void);
void func_80018DF8(void);
void func_8001901C(void);
void GameLoadGrandPrixScreen(void) asm("func_800195F4");
void GameLoadCourseAssets(void) asm("func_800196B4");
void func_8001989C(void);

void func_80019C04(void) {
    if (g_AssetLoadState != 0) {
        switch (g_MainState) {
        case 1:
            func_800180CC();
            break;
        case 2:
            func_80018344();
            break;
        case 3:
            func_80018484();
            break;
        case 4:
            func_80018588();
            break;
        case 5:
            func_800188B8(D_8009AEFC);
            break;
        case 6:
            func_80018A70(D_8009AEFC);
            break;
        case 7:
            func_80018C0C();
            break;
        case 8:
            func_80018DF8();
            break;
        case 9:
            func_8001901C();
            break;
        case 10:
            GameLoadGrandPrixScreen();
            break;
        case 11:
            GameLoadCourseAssets();
            break;
        case 12:
            func_8001989C();
            break;
        }
    }
}
