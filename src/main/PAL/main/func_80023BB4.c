#include "common.h"
#include "game/state.h"

extern u8 *D_8019C900;
extern s32 D_8019C7B4;

s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);
void func_800236C8(s32 arg0, s32 arg1);

void func_80023BB4(void) {
    register u8 *base asm("$16") = D_8019C900;
    register s32 h18 asm("$19") = 0x18;
    register s32 h48 asm("$17") = 0x48;
    register s32 color asm("$18") = 0x7F40;
    register s32 *scratch asm("$21") = (s32 *)0x1F800000;
    register s32 tmp asm("$20");
    s32 state;

    base += 0xCC;
    tmp = func_80017138((s32)base, *scratch, 0x24, 0x94, 0x3C, h18, 0, h48, color);
    tmp = func_80017138((s32)base, tmp, 0x24, 0xB4, 0x88, h18, 0x40, h48, color);
    tmp = func_80017138((s32)base, tmp, 0x24, 0xD4, 0x74, h18, 0, 0x60, color);
    tmp = func_80017138((s32)base, tmp, 0x24, 0xF4, 0x5C, h18, 0x74, 0x60, color);
    tmp = func_80017138((s32)base, tmp, 0x24, 0x114, 0x64, h18, 0, 0x78, color);
    tmp = func_80017138((s32)base, tmp, 0x24, 0x134, 0x1C, h18, 0xD0, 0x60, color);
    tmp = func_80017390((s32)base, tmp, 0x3F);

    state = g_GameMode;
    *scratch = tmp;
    if (state == 1) {
        func_800236C8(0x14, (D_8019C7B4 << 5) + 0x94);
    }
}
