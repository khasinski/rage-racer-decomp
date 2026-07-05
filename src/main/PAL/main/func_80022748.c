#include "common.h"

extern s32 D_801E40B8;
extern s32 D_8019C768;
extern s32 D_801E6C8C;
extern s32 D_801E42E4;

void func_80022324(void);

void func_80022748(void) {
    D_801E40B8 = 0x100;
    D_8019C768 = 0x80;
    D_801E6C8C = 0;
    D_801E42E4 = 0x15;
    func_80022324();
}
