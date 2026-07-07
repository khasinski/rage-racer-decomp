#include "common.h"

extern u8 *D_801E42D8;
extern u16 D_801E4130;
extern u8 *D_8019C900;

u8 *func_80033B7C(u8 *prim, s32 x, s32 y, s32 code, u16 arg4);
void *func_80017390(void *ot, void *packet, s32 arg2);

void func_80033C18(s32 x, s32 y, s32 value) {
    u8 **scratch;
    u8 *prim;
    s32 screenX;
    s32 screenY;
    s32 rawX;
    s32 rawY;
    s32 hundreds;
    s32 tenths;
    s32 tens;
    s32 ones;
    u16 color;

    hundreds = value / 100;
    rawX = *(u16 *)(D_801E42D8 + 0x140) + x;
    rawY = *(u16 *)(D_801E42D8 + 0x142) + y;
    color = D_801E4130;
    scratch = (u8 **)0x1F800000;
    prim = *scratch;

    tenths = value / 10;
    screenX = (s16)rawX;
    screenY = (s16)rawY;
    tens = tenths - (hundreds * 10);
    ones = value - (tenths * 10);

    prim = func_80033B7C(prim, screenX, screenY, hundreds, color);
    prim = func_80033B7C(prim, screenX + 8, screenY, tens, color);
    prim = func_80033B7C(prim, screenX + 0x10, screenY, ones, color);
    *scratch = func_80017390(D_8019C900 + 0xCC, prim, 9);
}
