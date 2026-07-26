#include "common.h"
#include "game/asset.h"
#include "game/race.h"

extern GameSceneAssetHeader *D_8019CAFC;

s32 func_80017C78(s32 assetIndex, void *dst);
s32 func_80017C2C(void);
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

void func_8001989C(void) {
    register GameSceneAssetHeader *header asm("$4");
    register void *dst asm("$5");
    register s32 offset asm("$2");

    switch (g_AssetLoadState) {
    case 1:
        dst = D_8019CAFC;
        offset = g_CourseIndex;
        __asm__ volatile("" : "=r"(offset) : "0"(offset));
        header = (GameSceneAssetHeader *)g_GrandPrixClass;
        if (func_80017C78(((s32)header << 3) + (offset << 1) + 0x58, dst) != 0) {
            header = D_8019CAFC;
            offset = header->offsets[0];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80017BD4(g_AssetBlockPtr);

            header = D_8019CAFC;
            offset = header->offsets[1];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_8004553C(g_AssetBlockPtr);

            header = D_8019CAFC;
            offset = header->offsets[2];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_8004550C(g_AssetBlockPtr);

            header = D_8019CAFC;
            offset = header->offsets[3];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80017948(g_AssetBlockPtr, 1);

            header = D_8019CAFC;
            offset = header->offsets[4];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_8002A6B0(g_AssetBlockPtr);

            header = D_8019CAFC;
            offset = header->offsets[5];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80017A6C(g_AssetBlockPtr);

            header = D_8019CAFC;
            offset = header->offsets[6];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80017948(g_AssetBlockPtr, 2);

            header = D_8019CAFC;
            offset = header->offsets[7];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80017AD0(g_AssetBlockPtr);

            header = D_8019CAFC;
            offset = header->offsets[8];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_8004121C(g_AssetBlockPtr);

            header = D_8019CAFC;
            offset = header->offsets[9];
            g_AssetBlockPtr = (u8 *)header + offset;
            func_80034E04(g_AssetBlockPtr);

            header = D_8019CAFC;
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
