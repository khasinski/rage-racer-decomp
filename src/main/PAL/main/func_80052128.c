#include "common.h"

extern s32 D_80082E70[];

void func_80016754(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_80052128(s32 arg0, s32 arg1, s32 arg2) {
    func_80016754(arg0, arg1, D_80082E70[arg2], 0x78CC);
}
