#include "common.h"

extern s32 D_8007FB04;
extern s32 D_8009B26C;
extern s32 D_8019CB0C;

INCLUDE_RODATA("asm/PAL/main/nonmatchings/main/func_80048ED8", func_80048ED8_rodata);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80048ED8", func_80048ED8);

void func_80046A2C(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0,
                   u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans,
                   u32 flags);
void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g,
                   u8 b, u8 alpha);
s32 func_80068568(s32 angle);

void func_80049418(s32 step, s32 wide, s32 drawLeft, s32 drawRight) {
    void *ot;
    s32 halfWidth;
    s32 y;
    s32 phase;
    s32 wave;
    s32 leftX;
    s32 intensityBias;
    s16 callY;
    u32 flags;
    s16 leftEdge;
    s16 rightEdge;
    u8 intensity;

    ot = *(void **)0x1F800004;
    if (step == 0) {
        D_8007FB04 = 0;
        return;
    }

    if (step < 0) {
        D_8007FB04 += step;
        if (D_8007FB04 < 0) {
            D_8007FB04 = 0;
        }
    }

    intensityBias = 65;
    if (wide != 0 || D_8019CB0C != 0) {
        halfWidth = 0x58;
    } else {
        halfWidth = 1;
    }

    y = 0x119;
    if (wide != 0) {
        y = 0x144;
    }
    phase = D_8007FB04 - 11;
    if (phase >= 0) {
        if (phase >= 11) {
            phase = 10;
        }

        leftX = (((u32)(phase * 0x250)) >> 5) + 0xFFE7;
        wave = func_80068568(D_8009B26C % 0x1000);
        intensity = (wave / 64) - intensityBias;
        leftEdge = leftX - halfWidth;
        D_8009B26C += 0x60;
        callY = y;
        flags = 0x19;

        func_80046A2C(ot, leftEdge, callY, 0x10, 0x20, 0x48, 0xB8,
                      0, 0, 0, 0x25A, 1, 0, flags);
        rightEdge = 0x1BF - leftX;
        func_80046A2C(ot, rightEdge, callY, 0x10, 0x20, 0x58, 0xB8,
                      0, 0, 0, 0x25A, 1, 0, flags);

        if (drawLeft != 0) {
            func_80047024(ot, leftEdge, callY, 0x10, 0x20,
                           0, intensity, 0, 0xFF);
        }
        if (drawRight != 0) {
            func_80047024(ot, rightEdge, callY, 0x10, 0x20,
                           0, intensity, 0, 0xFF);
        }
    }

    if (step > 0) {
        halfWidth = step;
        D_8007FB04 += halfWidth;
        if (D_8007FB04 >= 26) {
            D_8007FB04 = 25;
        }
    }
}
