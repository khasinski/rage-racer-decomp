#include "common.h"
#include "game/asset.h"

extern s32 D_8007BED8;
extern s32 D_8007C704;
extern GameAssetTripleHeader *D_8019C904;
extern void *D_8019C754;
extern void *D_801E8AB0;
extern void *D_801F17A8;

s32 func_80017C78(s32 assetIndex, void *dst);
void func_80042C94(void);
void func_8005B9CC(void);

s32 func_8001839C(void) {
    register s32 loadType asm("$16");

    if (D_8007BED8 != 0) {
        return 1;
    }

    loadType = 3;
    if (D_8007C704 == loadType) {
        D_8007C704 = 0;
        return 0;
    }

    func_80042C94();
    D_8007C704 = loadType;
    D_8007BED8 = 2;
    return 1;
}

s32 func_80018410(void) {
    register s32 loadType asm("$16");

    if (D_8007BED8 != 0) {
        return 1;
    }

    loadType = 3;
    if (D_8007C704 == loadType) {
        D_8007C704 = 0;
        return 0;
    }

    func_80042C94();
    D_8007C704 = loadType;
    D_8007BED8 = 1;
    return 1;
}

void func_80018484(void) {
    register GameAssetTripleHeader *header asm("$2");
    register s32 firstOffset asm("$3");
    register s32 secondOffset asm("$3");
    register s32 thirdOffset asm("$4");

    switch (D_8007BED8) {
    case 1:
        func_8005B9CC();
        D_8007BED8 = 2;
    case 2:
        if (func_80017C78(7, D_8019C904) != 0) {
            header = D_8019C904;
            firstOffset = *(volatile s32 *)&header->firstOffset;
            thirdOffset = *(volatile s32 *)&header->thirdOffset;
            D_801F17A8 = (void *)((u8 *)header + firstOffset);
            secondOffset = *(volatile s32 *)&header->secondOffset;
            __asm__ volatile("" ::: "memory");
            D_8007BED8 = 0;
            secondOffset = (s32)((u8 *)header + secondOffset);
            header = (GameAssetTripleHeader *)((u8 *)header + thirdOffset);
            __asm__ volatile("" ::: "memory");
            D_8019C754 = (void *)secondOffset;
            D_801E8AB0 = header;
        }
        break;
    }
}
