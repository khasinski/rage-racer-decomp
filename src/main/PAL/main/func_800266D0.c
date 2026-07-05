#include "common.h"

extern u8 *D_8019C900;
extern s32 D_801E428C;

s32 func_8002664C(s32 arg0);
void func_80046A2C(
    u8 *arg0,
    u32 arg1,
    u32 arg2,
    u32 arg3,
    u32 arg4,
    u32 arg5,
    u32 arg6,
    u32 arg7,
    u32 arg8,
    u32 arg9,
    u32 arg10,
    u32 arg11,
    u32 arg12,
    u32 arg13);

void func_800266D0(void) {
    u8 *ptr;
    s32 value;
    u32 one;
    u32 arg13;

    ptr = D_8019C900 + 0xCC;
    value = func_8002664C(0);
    one = 1;
    arg13 = 0x29;
    func_80046A2C(ptr, 0x74, 0x34, 0x58, 0x38, 0xA8, 0xA8, value, value, value, 0x1F, 0, one, arg13);
    func_80046A2C(ptr, 0x44, 0x70, 0xB8, 0x14, 0x48, 0xE8, value, value, value, 0x80, 0, one, arg13);
    value = func_8002664C(1);
    func_80046A2C(ptr, 0x5E, 0x90, 0x84, 0xC, 0, (D_801E428C * 12) + 0x9C, value, value, value, 0x12, 0, one, arg13);
}
