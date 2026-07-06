#include "common.h"

extern u8 D_801E4CF7;
extern s32 D_801E6F10;
extern s32 D_801E8AF8;
extern s32 D_8019CA6C;

void func_80019EBC(void) {
    s32 value = 1;
    s32 i = 0xFF;
    u8 *ptr = &D_801E4CF7;

    do {
        *ptr = value;
        i--;
        ptr--;
    } while (i >= 0);

    D_801E6F10 = 0;
    D_801E8AF8 = 0;
    D_8019CA6C = 0;
}
