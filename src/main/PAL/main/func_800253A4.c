#include "common.h"

extern u8 *D_8019C900;

s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_80023750(s32 arg0);

void func_800253A4(void) {
    s32 base = (s32)D_8019C900;
    s32 color = 0x7F40;
    s32 y48 = 0x48;
    s32 h18 = 0x18;
    s32 w0c = 0xC;
    s32 *scratch = (s32 *)0x1F800000;
    s32 next;

    base += 0xCC;
    next = *scratch;
    next = func_80017138(base, next, 0x9A, 0x88, w0c, h18, 0xC8, y48, color);
    next = func_80017138(base, next, 0x9A, 0xB8, w0c, h18, 0xD4, y48, color);
    next = func_80017138(base, next, 0xA6, 0xA0, w0c, h18, 0xE0, y48, color);
    *scratch = func_80017138(base, next, 0x8E, 0xA0, w0c, h18, 0xEC, y48, color);
    func_80023750(3);
}
