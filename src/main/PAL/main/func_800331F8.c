#include "common.h"

void func_80033F30(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_800331F8(s32 arg0) {
    s32 arg3 = 0x78CC;

    if (arg0 < 0x5DC) {
        arg3 = 0x7811;
    }

    func_80033F30(0xE, 0xD2, arg0, arg3);
}
