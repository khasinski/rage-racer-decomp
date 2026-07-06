#include "common.h"
#include "game/asset.h"

extern char D_80010ADC[];
extern char D_80010AFC[];
extern char D_80010B08[];
extern s16 D_8007C700;
extern char *D_8007C48C[];
extern GameCdLoadEntry D_801E6834[];

void func_8001674C(char *arg0, ...);
s32 func_80027688(s32 arg0, void *arg1, s32 arg2);
s32 func_80027790(s32 arg0, s32 arg1);
s32 func_8006A534(s32 arg0, s32 arg1);
void func_8006A6DC(s32 arg0, void *arg1);

s32 func_80017C78(s32 assetIndex, void *dst) {
    register void *dstReg asm("$6");
    s32 result;
    s32 size;

    dstReg = dst;

    switch (D_8007C700) {
    case 0:
        func_8001674C(D_80010ADC, D_8007C48C[assetIndex]);
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
        if (func_80027688((D_801E6834[assetIndex].size + 0x7FF) >> 11, dstReg, 0x80) != 0) {
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
        func_8001674C(D_80010B08, D_8007C48C[assetIndex]);
        D_8007C700 = 0;
        break;
    }

    return 0;
}
