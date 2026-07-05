#include "common.h"

extern s32 D_8007F5F8;
extern s32 D_8007F600;
extern s32 D_8007F604;
extern s32 D_8007F608;
extern s32 D_8007F60C;
extern u8 D_8009B1B0;

void func_80042C28(void) {
    if (D_8007F5F8 != 0) {
        u8 value;

        value = D_8009B1B0;
        D_8007F608 = 4;
        D_8007F5F8 = 0;
        D_8007F604 = 1;
        D_8007F60C = 0;
        D_8007F600 = value;
    } else {
        D_8007F604 = 3;
        D_8007F60C = 0;
    }
}
