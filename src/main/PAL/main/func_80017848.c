#include "common.h"

extern u8 D_8007C464[];

s32 func_80017848(s32 arg0, s32 arg1) {
    return D_8007C464[arg0] + arg1;
}
