#include "common.h"
extern u8 *D_8019C900;
void func_80046A2C(void *arg0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10, s32 a11, s32 a12, s32 a13);
void func_800218A0(s32 arg0) {
    if (arg0 >= 256) {
        arg0 = 0xFF;
    }
    arg0 >>= 1;
    func_80046A2C(D_8019C900 + 204, 0x50, 0x6C, 0xA0, 0x18, 0, 0x28, arg0, arg0, arg0, 0xC, 0, 1, 0x29);
}
