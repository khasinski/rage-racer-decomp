#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "psyq/gpu.h"

extern u8 *D_8019CAFC;
extern Rect D_8007BEDC;
extern Rect D_8007BEE4;
extern u16 D_801E444C[];
extern u16 D_801E6F2C[];
void func_80042C94(void);
s32 func_80017C78(s32 assetIndex, void *dst);
void func_8001A3C0(void *arg0);
void func_8001A498(void);
void func_80034DCC(void *arg0);
void func_8005B768(s32 arg0, void *arg1, void *arg2, s32 arg3);
s32 func_8005B89C(void);
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void DrawSync(s32 mode) asm("func_800658FC");
extern s32 D_801E4B30;
extern GameAssetTripleHeader *D_8019C904;
extern void *D_8019C754;
extern void *D_801E8AB0;
void func_8005B9CC(void);

void func_800180CC(void) {
    register u8 *loaded asm("$16");
    register u8 *base asm("$17");
    register u8 *next asm("$2");
    register s32 nextState asm("$3");

    switch (g_AssetLoadState) {
    case 1:
        base = (u8 *)g_LoadBuffer;
        loaded = (u8 *)func_80017C78(1, base);
        if (loaded != 0) {
            func_8001A498();
            __asm__ volatile("" ::: "memory");
            next = loaded + (s32)base;
            g_AssetBlockPtr = next;
            g_AssetLoadState = 2;
        }
        break;
    case 2:
        loaded = (u8 *)func_80017C78(2, g_AssetBlockPtr);
        if (loaded != 0) {
            nextState = 3;
            next = g_AssetBlockPtr;
            goto setNextBuffer;
        }
        break;
    case 3:
        if (func_80017C78(3, D_8019CAFC) != 0) {
            func_8005B768(0, g_AssetBlockPtr, D_8019CAFC, 0);
            g_AssetLoadState = 4;
        }
        break;
    case 4:
        if ((func_8005B89C() << 16) != 0) {
            g_AssetLoadState = 5;
        }
        break;
    case 5:
        loaded = (u8 *)func_80017C78(4, D_8019CAFC);
        if (loaded != 0) {
            func_80034DCC(D_8019CAFC);
            next = D_8019CAFC;
            __asm__ volatile("" : "=r"(next) : "0"(next));
            nextState = 6;
setNextBuffer:
            g_AssetLoadState = nextState;
            __asm__ volatile("" ::: "memory");
            next = loaded + (s32)next;
            D_8019CAFC = next;
        }
        break;
    case 6:
        if (func_80017C78(5, D_8019CAFC) != 0) {
            register u8 *finalBase asm("$2");

            func_8001A3C0(D_8019CAFC);
            StoreImage(&D_8007BEDC, D_801E444C);
            StoreImage(&D_8007BEE4, D_801E6F2C);
            DrawSync(0);
            finalBase = D_8019CAFC;
            D_801E444C[0] = 0;
            g_AssetLoadState = 0;
            D_8019C904 = (GameAssetTripleHeader *)finalBase;
        }
        break;
    }
}

s32 func_800182D0(void) {
    register s32 state asm("$16");

    if (g_AssetLoadState != 0) {
        return 1;
    }

    state = 2;
    if (g_MainState == state) {
        g_MainState = 0;
        return 0;
    }

    func_80042C94();
    g_MainState = state;
    g_AssetLoadState = 1;
    return 1;
}

void func_80018344(void) {
    if (g_AssetLoadState == 1) {
        if (func_80017C78(6, D_8019C904) != 0) {
            g_AssetLoadState = 0;
            D_801E4B30 = (s32)D_8019C904;
        }
    }
}

s32 func_8001839C(void) {
    register s32 loadType asm("$16");

    if (g_AssetLoadState != 0) {
        return 1;
    }

    loadType = 3;
    if (g_MainState == loadType) {
        g_MainState = 0;
        return 0;
    }

    func_80042C94();
    g_MainState = loadType;
    g_AssetLoadState = 2;
    return 1;
}

s32 func_80018410(void) {
    register s32 loadType asm("$16");

    if (g_AssetLoadState != 0) {
        return 1;
    }

    loadType = 3;
    if (g_MainState == loadType) {
        g_MainState = 0;
        return 0;
    }

    func_80042C94();
    g_MainState = loadType;
    g_AssetLoadState = 1;
    return 1;
}

void func_80018484(void) {
    register GameAssetTripleHeader *header asm("$2");
    register s32 firstOffset asm("$3");
    register s32 secondOffset asm("$3");
    register s32 thirdOffset asm("$4");

    switch (g_AssetLoadState) {
    case 1:
        func_8005B9CC();
        g_AssetLoadState = 2;
    case 2:
        if (func_80017C78(7, D_8019C904) != 0) {
            header = D_8019C904;
            firstOffset = *(volatile s32 *)&header->firstOffset;
            thirdOffset = *(volatile s32 *)&header->thirdOffset;
            g_AssetBlockPtr = (void *)((u8 *)header + firstOffset);
            secondOffset = *(volatile s32 *)&header->secondOffset;
            __asm__ volatile("" ::: "memory");
            g_AssetLoadState = 0;
            secondOffset = (s32)((u8 *)header + secondOffset);
            header = (GameAssetTripleHeader *)((u8 *)header + thirdOffset);
            __asm__ volatile("" ::: "memory");
            D_8019C754 = (void *)secondOffset;
            D_801E8AB0 = header;
        }
        break;
    }
}
