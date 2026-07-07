#include "common.h"

extern s32 D_8007FB3C;
extern s32 D_8019CB0C;

void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha);

s32 func_8005026C(s32 arg0) {
    register void *scratch asm("$23");
    register s32 delta asm("$22");
    register s32 value asm("$16");
    register s32 y1 asm("$17");
    register s32 red asm("$21");
    register s32 green asm("$20");
    register s32 blue asm("$19");
    register s32 alpha asm("$18");
    register s32 temp asm("$2");
    register s32 zero asm("$5");
    register void *callScratch asm("$4");
    register s32 yArg asm("$6");

    scratch = *(void **)0x1F800004;
    delta = arg0;

    if (delta == 0) {
        asm("sw $0,%0" : "=m"(D_8007FB3C));
        return temp;
    } else {
        if (delta < 0) {
            temp = D_8007FB3C;
            temp = delta + temp;
            D_8007FB3C = temp;
            if (temp < 0) {
                D_8007FB3C = 0;
            }
        }

        value = D_8007FB3C;
        if (value >= 0 && D_8019CB0C == 0) {
            if (value >= 0x10) {
                value = 0xF;
            }
            callScratch = scratch;
            zero = 0;
            asm("" : "=r"(callScratch) : "0"(callScratch));
            asm("" : "=r"(zero) : "0"(zero));
            value = ((u32)(value << 9) >> 5) + 0xFF10;
            yArg = (s16)value;
            y1 = 0xF0;
            red = 0x95;
            green = 0x25;
            blue = 0x1E;
            alpha = 0xFF;
            func_80047024(callScratch, zero, yArg, 0x140, y1, red, green, blue, alpha);
            callScratch = scratch;
            zero = 0;
            asm("" : "=r"(zero) : "0"(zero));
            value = y1 - value;
            value <<= 0x10;
            yArg = value >> 0x10;
            func_80047024(callScratch, zero, yArg, 0x140, y1, red, green, blue, alpha);
        }

        if (delta > 0) {
            temp = D_8007FB3C;
            temp = delta + temp;
            D_8007FB3C = temp;
            if (temp >= 0x1A) {
                D_8007FB3C = 0x19;
            }
        }
    }

    value = D_8007FB3C;
    return value;
}
