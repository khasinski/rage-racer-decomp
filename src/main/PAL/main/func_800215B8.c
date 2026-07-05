#include "common.h"

extern u8 D_80010E68;

void func_80016B7C(u32 arg0, u32 arg1, void *arg2, u32 arg3, u32 arg4);

void func_800215B8(s32 arg0) {
    if (arg0 >= 0x100) {
        arg0 = 0xFF;
    }
    arg0 >>= 1;
    func_80016B7C(0x28, 0x40, &D_80010E68, 0x7812, arg0);
}
