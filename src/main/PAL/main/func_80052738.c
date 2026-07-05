#include "common.h"
extern s32 D_801E40B8;
extern u8 D_80011B60;
void func_80016754(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void func_80052738(void) {
    if (D_801E40B8 & 8) {
        func_80016754(0x74, 0xEC, &D_80011B60, 0x78CC);
    }
}
