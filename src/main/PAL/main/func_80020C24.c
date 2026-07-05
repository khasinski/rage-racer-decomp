#include "common.h"

extern s32 D_801E40B8;
extern s32 D_8019C768;
extern s32 D_801E428C;
extern s32 D_8009E6A4;
extern s32 D_8019CB74;
extern s16 D_8009E834;
extern s32 D_801E419C;
extern s32 D_801E42E4;
extern s32 D_801F17B0;
extern s32 D_8019CE0C;
extern s32 D_801E6DA0;
extern s32 D_801E6C78;
extern s32 D_8007BEEC[][6][3];
extern s32 D_8007BEF4[][6][3];
extern s32 D_8007C00C[];

void func_80020C24(void) {
    s32 mode;
    s32 car;
    s32 value;

    D_801E40B8 = 0x100;
    D_8019C768 = 0x80;

    mode = D_801E428C;
    car = D_8009E6A4;
    D_8019CB74 = 0;
    D_801F17B0 = D_8007BEEC[mode][car][D_8009E834 - 1];
    D_801E42E4 = 0x13;

    if (D_801E419C != 0) {
        D_8019CE0C = D_8007C00C[car];
    } else {
        D_8019CE0C = 0;
    }

    value = D_8007BEF4[D_801E428C][D_8009E6A4][0] / 80;
    D_801E6DA0 = value;
    if (value <= 0) {
        D_801E6DA0 = 1;
    }

    value = D_8007C00C[D_8009E6A4] / 250;
    D_801E6C78 = value;
    if (value <= 0) {
        D_801E6C78 = 1;
    }
}
