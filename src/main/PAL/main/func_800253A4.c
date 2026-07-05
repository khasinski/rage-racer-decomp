#include "common.h"

extern u8 *D_8019C900;

s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_80023750(s32 arg0);

void func_800253A4(void) {
    register s32 base asm("$16") = (s32)D_8019C900;
    register s32 color asm("$17") = 0x7F40;
    register s32 y48 asm("$18") = 0x48;
    register s32 h18 asm("$19") = 0x18;
    register s32 w0c asm("$20") = 0xC;
    register s32 *scratch asm("$21") = (s32 *)0x1F800000;
    s32 next;

    base += 0xCC;
    next = func_80017138(base, *scratch, 0x9A, 0x88, w0c, h18, 0xC8, y48, color);
    next = func_80017138(base, next, 0x9A, 0xB8, w0c, h18, 0xD4, y48, color);
    next = func_80017138(base, next, 0xA6, 0xA0, w0c, h18, 0xE0, y48, color);
    *scratch = func_80017138(base, next, 0x8E, 0xA0, w0c, h18, 0xEC, y48, color);
    func_80023750(3);
}
