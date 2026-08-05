#include "common.h"

extern s32 D_8007FB3C;
extern s32 g_MenuAltLayout;

void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha);

s32 DrawClassChangeCurtain(s32 arg0);
s32 DrawClassChangeCurtain(s32 arg0) {
    void *scratch;
    s32 delta;
    register s32 value asm("$16");
    s32 y1;
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    s32 temp;
    register s32 zero asm("$5");
    register void *callScratch asm("$4");
    s32 yArg;

    scratch = *(void **)0x1F800004;
    delta = arg0;

    if (delta == 0) {
        D_8007FB3C = 0;
        return temp;
    } else {
        if (delta < 0) {
            temp = D_8007FB3C;
            { s32 rel = temp; temp = delta + rel; }
            D_8007FB3C = temp;
            if (temp < 0) {
                D_8007FB3C = 0;
            }
        }

        value = D_8007FB3C;
        if (value >= 0 && g_MenuAltLayout == 0) {
            if (value >= 0x10) {
                value = 0xF;
            }
            callScratch = scratch;
            zero = 0;
            asm("" : "=r"(zero) : "0"(zero));
            value = ((u32)(value << 9) / 32) + 0xFF10;
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
            { s32 rel = temp; temp = delta + rel; }
            D_8007FB3C = temp;
            if (temp >= 0x1A) {
                D_8007FB3C = 0x19;
            }
        }
    }

    value = D_8007FB3C;
    return value;
}
