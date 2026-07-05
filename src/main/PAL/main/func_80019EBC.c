#include "common.h"

extern u8 D_801E4CF7;
extern s32 D_801E6F10;
extern s32 D_801E8AF8;
extern s32 D_8019CA6C;

void func_80019EBC(void) {
    register s32 value asm("$4") = 1;
    register s32 i asm("$3") = 0xFF;
    register u8 *ptr asm("$2") = &D_801E4CF7;

    do {
        *ptr = value;
        i--;
        ptr--;
    } while (i >= 0);

    D_801E6F10 = 0;
    D_801E8AF8 = 0;
    D_8019CA6C = 0;
}
