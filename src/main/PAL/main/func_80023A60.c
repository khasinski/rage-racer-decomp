#include "common.h"
#include "game/state.h"

extern u8 *D_8019C900;
s32 func_8001720C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);
extern u32 D_8019C7B0;
extern u32 D_801E42A0;
extern s32 D_801E42E0;
extern s32 D_801E42E4;
void func_80023A60(s32 arg0, s32 arg1);
void func_80023BB4(void);

void func_80023A60(s32 color, s32 arg1) {
    u8 *base;
    s32 *scratch;
    s32 next;
    s32 width;
    s32 height;
    s32 scratchValue;

    base = D_8019C900 + 0xCC;
    if (color < 0) {
        color = 0;
    } else if (color >= 0x100) {
        color = 0xFF;
    }

    width = 0x140;
    height = 0x1E0;
    scratch = (s32 *)0x1F800000;
    scratchValue = *scratch;
    next = func_8001720C(base, scratchValue, 0, 0, width, height, color, color, color);
    *scratch = func_80017390(base, next, arg1);
}

void func_80023B08(u32 arg0) {
    D_8019C7B0 = arg0;
    g_GameMode = 0;
    D_801E42A0 = 8;
}

void func_80023B2C(void) {
    D_801E42E0 += D_801E42A0;

    if (D_801E42E0 < 0) {
        D_801E42A0 = 0;
        g_GameMode = 1;
    } else if (D_801E42E0 >= 0x101) {
        D_801E42E4 = D_8019C7B0;
    }

    func_80023A60(D_801E42E0, 0x49);
    func_80023BB4();
}
