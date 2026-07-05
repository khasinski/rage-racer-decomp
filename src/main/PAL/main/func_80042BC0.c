#include "common.h"

extern s32 D_8007F600;
extern s32 D_8007F604;
extern s32 D_8007F608;
extern s32 D_8007F60C;

void func_80042BC0(s32 arg0) {
    D_8007F600 = arg0 & 0xFF;
    D_8007F608 = 0;
    D_8007F604 = -1;
    D_8007F60C = 0;
}
