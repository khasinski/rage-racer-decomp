#include "common.h"

extern u16 *D_801E5020;

u32 func_800414A0(s32 arg0, s32 arg1) {
    arg1 = (arg1 << 5) + arg0;
    return D_801E5020[arg1] >> 10;
}
