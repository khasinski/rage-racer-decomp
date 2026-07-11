#include "common.h"

extern s32 D_801E428C;
extern s32 D_801E4BA8;
extern s16 D_8019CABC;
extern s16 D_801E4DAC;
extern s32 D_8019C70C[][4][2];
extern s32 D_8009E858[];
extern char D_80010E20[];
extern char D_80010E28[];

void func_80016EA0(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void *func_80021CD4(void *dst, s32 value);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_800201D4", func_800201D4);

void func_800204F4(s32 arg0) {
    register s32 base asm("$21");
    register s32 i asm("$16");
    register s32 *times asm("$18");
    register s32 *selectedPtr asm("$22");
    register s32 count asm("$20");
    register s32 x asm("$19");
    register s32 textPos asm("$17");
    char text[24];
    s32 color;

    base = arg0;
    func_80016EA0(0x10, base + 0x80, D_80010E20, 0x7812);

    text[0] = 0x54;
    text[1] = 0x2F;
    func_80021CD4(&text[2], D_801E4BA8);

    color = 0x7812;
    if (D_8019C70C[D_8019CABC][D_801E428C][D_801E4DAC] == D_801E4BA8) {
        color = 0x784C;
    }
    func_80016EA0(0x14, base + 0x90, text, color);

    func_80016EA0(0x10, base + 0xA4, D_80010E28, 0x7812);

    count = 6;
    if (D_801E428C != 3) {
        count = 3;
    }

    i = 0;
    if (count != 0) {
        times = D_8009E858;
        selectedPtr = D_8009E858;
        do {
            x = 0xB0;
            if (i < 3) {
                x = 0x14;
            }
            text[0] = i + 0x31;
            {
                register s32 offset asm("$3");
                register void *dst asm("$4");
                register s32 value asm("$5");

                offset = (i % 3) * 0xC;
                dst = &text[2];
                value = *times;
                asm volatile(
                    ".word 0x26A200B0\n\t"
                    ".word 0x0C008735\n\t"
                    ".word 0x00628821"
                    : "=r"(textPos)
                    : "r"(offset), "r"(dst), "r"(base), "r"(value)
                    : "$2", "$31", "memory");
            }
            color = 0x7812;
            if (*(s16 *)((char *)selectedPtr - 0x22) == i) {
                color = 0x784C;
            }
            func_80016EA0(x, textPos, text, color);
            i++;
            times++;
        } while (i < count);
    }
}
