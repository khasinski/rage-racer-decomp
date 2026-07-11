#include "common.h"

extern u8 *D_8019C900;
extern s32 D_801E40B8;
extern s32 *D_8007D99C[];
extern s16 D_8007D9E8[];

void func_80027874(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_80016EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);

void func_80027D84(s32 arg0) {
    register s32 index asm("$17");
    register s32 *entry asm("$16");
    register s32 x asm("$19");
    register s32 y asm("$18");
    register s16 *table asm("$20");
    register s32 one asm("$21");
    register s32 temp asm("$2");
    s32 next;
    s32 base;
    s32 delta;

    index = arg0;
    asm("" : "=r"(index) : "0"(index));
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
            temp = *(u8 *)((s32)entry + 4);
            entry += 2;
        } while (temp != 0);
    }

    base = (s32)D_8019C900 + 0xCC;
    next = *(s32 *)0x1F800000;
    if (index == 6 || index == 8 || index == 0xA || index == 0xC) {
        next = func_80016EC4(base, next, 0xDE, 0x60, 0xC, 0x18, 0x84, 0x48, 0x7F81);
    }
    if (index == 7 || index == 9 || index == 0xB || index == 0xD) {
        next = func_80016EC4(base, next, 0xAC, 0x60, 0xC, 0x18, 0x84, 0x48, 0x7F81);
    }
    if (index == 5 && (D_801E40B8 & 0x10) != 0) {
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
