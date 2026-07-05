#include "common.h"

extern u32 D_8009AF58;
extern u32 D_8009AF5C;
extern u32 D_8009AF60;
extern u32 D_8009AF64;
extern u32 D_8009AF68;

void func_8001EB14(u32 arg0) {
    u32 step;
    u32 temp;

    step = 0x28000;
    D_8009AF5C = arg0;
    arg0 += step;
    D_8009AF60 = arg0;
    arg0 += step;
    temp = arg0 + 0x2D00;
    D_8009AF64 = arg0;
    arg0 += 0x5A00;
    D_8009AF68 = temp;
    D_8009AF58 = arg0;
}
