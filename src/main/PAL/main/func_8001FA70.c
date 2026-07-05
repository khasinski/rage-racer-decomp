#include "common.h"

extern s32 D_8009EC8C;
extern s32 D_801E42E0;
extern s32 D_801E40B8;
extern s32 D_801E42A0;
extern s32 D_801E4BB0;
extern s32 D_801F179C;
extern s32 D_8019CB6C;
extern s16 D_801E4DAC;
extern s32 D_8009E6A4;
extern u8 *D_8019C8FC;

void func_800458CC(void *arg0);
void func_80034F74(void);

void func_8001FA70(void) {
    s32 mode;

    D_801E42E0 = 0xFF;
    D_801E40B8 = 0;
    D_801E42A0 = -4;

    if (D_8009EC8C != 0) {
        D_801F179C = (D_801E4BB0 & -2) + 2;
    } else {
        D_801F179C = 0;
        D_8019CB6C = D_801E4BB0 - 2;
    }

    if (!(D_801F179C < D_8019CB6C)) {
        D_801F179C = 0;
    }

    if (D_801E4DAC != 0) {
        mode = D_8009E6A4;
        if (mode != 5) {
            func_800458CC(D_8019C8FC - 1800);
        }
    } else {
        mode = D_8009E6A4;
        if (mode != 5) {
            func_800458CC(D_8019C8FC - 3000);
        }
    }

    func_80034F74();
}
