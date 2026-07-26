#include "common.h"
#include "game/state.h"
#include "game/asset.h"
#include "psyq/cd.h"

extern s16 D_8007C700;
void func_80027634(void);
s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
extern char D_80010ADC[];
extern char D_80010AFC[];
extern char D_80010B08[];
extern GameCdLoadEntry D_801E6834[];
void func_8001674C(char *arg0, ...);
s32 func_80027688(s32 arg0, void *arg1, s32 arg2);
s32 func_80027790(s32 arg0, s32 arg1);
void func_8006A6DC(s32 arg0, void *arg1);
s32 func_80017C78(s32 arg0, s32 arg1);
extern char D_80010B38[];
extern char D_80010B44[];
extern char D_80010B58[];
extern char D_80010B68[];
extern char D_80010B80[];
extern char D_80010B8C[];
extern GameCdLoadEntry D_8007C6A8[];
extern s32 D_8009F0B8[];
void func_8001A3C0(void *arg0);

void func_80017BD4(u32 value) {
    *(u32 *)0x8019C9A8 = value;
}

void func_80017BE4(void) {
    if (D_8007C700 == 4) {
        func_80027634();
    }

    D_8007C700 = 0;
    g_AssetLoadState = 0;
    g_MainState = 0;
}

s32 func_80017C2C(void) {
    u8 value;

    if (func_8006A534(1, 0) == 0) {
        return 0;
    }

    value = 7;
    return func_8006A5A4(0xE, &value, 0);
}

s32 GameLoadAsset(s32 assetIndex, void *dst) {
    s32 result;
    s32 size;

    switch (D_8007C700) {
    case 0:
        func_8001674C(D_80010ADC, g_AssetPaths[assetIndex], dst);
        if (func_8006A534(1, 0) != 0) {
            D_8007C700 = 1;
        }
        return 0;

    case 1:
        func_8006A6DC(2, &D_801E6834[assetIndex]);
        D_8007C700 = 2;
        return 0;

    case 2:
        if (func_8006A534(1, 0) != 0) {
            D_8007C700 = 3;
        }
        return 0;

    case 3:
        if (func_80027688((D_801E6834[assetIndex].size + 0x7FF) >> 11, dst, 0x80) != 0) {
            D_8007C700 = 4;
        }
        return 0;

    case 4:
        result = func_80027790(1, 0);
        if (result == 0) {
            D_8007C700 = 5;
            return 0;
        }
        if (result == -1) {
            D_8007C700 = 6;
        }
        return 0;

    case 5:
        size = (D_801E6834[assetIndex].size >> 2) << 2;
        func_8001674C(D_80010AFC, size);
        D_8007C700 = 0;
        return size;

    case 6:
        func_8001674C(D_80010B08, g_AssetPaths[assetIndex], dst);
        D_8007C700 = 0;
        break;
    }

    return 0;
}

void func_80017E48(s32 arg0, s32 arg1) {
    while (func_80017C78(arg0, arg1) == 0) {
    }
}

void func_80017E8C(void) {
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

    func_8001674C(D_80010ADC, D_80010B38, D_8009F0B8);
    if (DsSearchFile(&stack.file, D_80010B38) == 0) {
        func_8001674C(D_80010B44, D_80010B38);
    }

    one = 1;
    do {
        func_8006A5A4(2, &stack.file, 0);
        func_80027688(one, D_8009F0B8, 0x80);
        do {
            status = func_80027790(1, 0);
        } while (status > 0);
    } while (status != 0);

    func_8001674C(D_80010B58, one);
    base = CdPosToInt_Local(&stack.file);
    src = D_8009F0B8;
    dst = D_801E6834;
    for (i = 0; i < 135; i++) {
        CdIntToPos(base + *src, (CdlLOC *)dst);
        dst->size = src[1];
        src += 2;
        dst++;
    }

    func_8001674C(D_80010B68, D_80010B80);
    if (DsSearchFile(&stack.file, D_80010B80) == 0) {
        func_8001674C(D_80010B44, D_80010B80);
        goto loadSmallTable;
    }

    func_8001674C(D_80010B8C);
loadSmallTable:
    base = CdPosToInt_Local(&stack.file);
    smallSrc = D_8007C6A8;
    for (i = 0; i < 11; i++) {
        CdIntToPos(base + smallSrc->position, (CdlLOC *)smallSrc);
        smallSrc++;
    }
}

void func_80018038(void) {
    void *ptr;

    func_80017E8C();
    ptr = &D_8009F0B8;
    func_80017E48(0, (s32)ptr);
    func_8001A3C0(ptr);
}

s32 func_80018078(void) {
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
