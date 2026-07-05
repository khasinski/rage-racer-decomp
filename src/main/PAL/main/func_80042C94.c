#include "common.h"

extern s32 D_8007F600;
extern s32 D_8007F604;
extern s32 D_8007F608;
extern s32 D_8007F60C;
extern u8 D_8009B1B0;

void func_80042C94(void) {
    D_8007F600 = -1;
    D_8007F604 = -1;
    D_8007F608 = 0;
    D_8007F60C = 0;
    D_8009B1B0 = 2;
}
