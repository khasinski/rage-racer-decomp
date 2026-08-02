#include "common.h"
#include "game/state.h"

extern u8 *volatile g_DrawBuffer asm("D_8019C900");
s32 func_80017138(s32 base, s32 prim, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 color);
s32 AddTilePrim(s32 base, s32 prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b) asm("func_80032F34");
void func_800236C8(s32 a, s32 b);
void func_80023750(s32 a);
void func_8002390C(void);
s32 func_800279EC(s32 base, s32 prim, s32 a, s32 b);
extern s32 *D_8007D99C[];
extern s16 D_8007D9E8[];
void func_80027874(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_80016EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);

void DrawMemoryCardScreen(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80027A84");
void DrawMemoryCardScreen(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 base = ((s32) g_DrawBuffer) + 0xCC;
    s32 *scratch = (s32 *) 0x1F800000;
    s32 next;
    s32 i;
    s32 y;

    next = func_80017138(base, *scratch, 0x24, 0x38, 0x20, 0x18, 0xA0, 0x90, 0x7F40);
    if (arg1 != 0) {
        next = func_80017138(base, next, 0x24, 0x58, 0x24, 0x18, 0xCC, 0x90, 0x7F40);
    }
    if (arg1 != 0) {
        y = 0x78;
    } else {
        y = 0x58;
    }
    next = func_80017138(base, next, 0x24, y, 0x1C, 0x18, 0xD0, 0x60, 0x7F40);
    next = func_80017138(base, next, 0x48, 0xB8, 0x10, 0x10, 0, 0xC8, 0x7F40);
    next = func_80017138(base, next, 0x68, 0xB8, 0x34, 0x10, 0x10, 0xC8, 0x7F40);
    next = func_80017138(base, next, 0xB0, 0xB8, 0x14, 0x10, 0x44, 0xC8, 0x7F40);
    *scratch = next;
    func_800236C8(0x14, (arg2 << 5) + 0x38);
    func_80023750(arg1 + 5);
    func_8002390C();

    base = ((s32) g_DrawBuffer) + 0xD8;
    next = AddTilePrim(base, *scratch, 0x5D, 0x3C, 0xE4, 0x40, 0, 0, 0);
    next = AddTilePrim(base, next, 0x5C, 0x3A, 0xE5, 0x44, 0xFF, 0xFF, 0xFF);
    for (i = 0; i < 3; i++) {
        next = func_800279EC(base, next, 0x3E, 0xD0 + i * 0x30);
    }

    if (arg0 != 0) {
        next = AddTilePrim(base, next, 0x3C, ((arg3 * 3) << 4) + 0xCC, 0xC8, 0x28, 0x89, 0xFF, 0x76);
    }
    next = AddTilePrim(base, next, 0, 0, 0x140, 0xF0, 0x85, 0x15, 0xE);
    *(s32 *)0x1F800000 = next;
}

void DrawMemoryCardMessage(s32 arg0) asm("func_80027D84");
void DrawMemoryCardMessage(s32 arg0) {
    s32 index;
    s32 *entry;
    s32 x;
    s32 y;
    s16 *table;
    s32 one;
    register s32 temp asm("$2");
    s32 next;
    s32 base;
    s32 delta;

    index = arg0;
    entry = D_8007D99C[index];
    x = 0x60;
    y = 0x40;
    if ((u32)(index - 0x10) >= 2 && index != 0x12) {
        one = 1;
        table = D_8007D9E8;
        temp = 1;
        do {
            temp = (u8)temp;
            if (temp != one) {
                x = table[temp];
                y = 0x60;
            }
            func_80027874(x, y, entry[0], 0x7F81);
            temp = ((u8 *)entry)[4];
            entry += 2;
        } while (temp != 0);
    }

    base = (s32)g_DrawBuffer + 0xCC;
    next = *(s32 *)0x1F800000;
    if (index == 6 || index == 8 || index == 0xA || index == 0xC) {
        next = func_80016EC4(base, next, 0xDE, 0x60, 0xC, 0x18, 0x84, 0x48, 0x7F81);
    }
    if (index == 7 || index == 9 || index == 0xB || index == 0xD) {
        next = func_80016EC4(base, next, 0xAC, 0x60, 0xC, 0x18, 0x84, 0x48, 0x7F81);
    }
    if (index == 5 && (g_SceneTimer & 0x10) != 0) {
        next = func_80016EC4(base, next, 0x108, 0x60, 0xC, 0x18, 0x90, 0x48, 0x7F81);
    }
    delta = index - 0x10;
    if ((u32)delta < 2 || index == 0x12) {
        next = func_80016EC4(base, next, x, y, 0x6C, 0x18, 0, delta * 0x18, 0x7F81);
        next = func_80017390(base, next, 0x3F);
    } else {
        next = func_80017390(base, next, 0x3D);
    }
    *(s32 *)0x1F800000 = next;
}
