#include "common.h"
#include "game/state.h"

extern s32 D_8007FB04;
extern s32 D_8009B268;
extern s32 D_8009B26C;
extern s32 g_MenuAltLayout asm("D_8019CB0C");

void func_80046A2C(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0,
                   u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans,
                   u32 flags);
void func_80046BA0(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, s16 x2, u16 y2,
                   u8 r, u8 g, u8 b, s32 semiTrans, u32 arg11);
void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g,
                   u8 b, u8 alpha);
void func_8004711C(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g,
                   u8 b, u8 alpha);
void func_80047460(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, u8 r, u8 g, u8 b, u8 alpha);
s32 func_80068568(s32 angle);

void func_80048ED8(u8 x, s32 useFlag) {
    void *ot;
    void *scratch;
    s32 gray;
    s32 alpha;
    s32 angle;
    s32 color;
    s32 zero;
    s32 xTest;
    s32 yLarge;
    s32 ySmall;

    scratch = *(void **)0x1F800004;

    switch ((u8)x) {
    case 4:
        x = 0xB8;
        break;
    case 3:
        x = 0xC7;
        break;
    case 2:
        x = 0xD7;
        break;
    case 1:
        x = 0xE7;
        break;
    case 0:
        x = 0xF7;
        break;
    }

    if (useFlag != 0) {
        if (g_AnimTimer & 2) {
            alpha = 0xFF;
        } else {
            alpha = 0x60;
        }
    } else {
        angle = D_8009B268;
        color = func_80068568(angle % 0x1000);
        if (color < 0) {
            color += 0x3F;
        }
        alpha = (color >> 6) - 0x41;
    }

    gray = 0xB4;
    ot = (void *)((s32)scratch + 8);

    func_80046A2C(ot, 0xBC, 0x50, 0x14, 0x10, 0, 0xB4, 0, 0, 0, 0x244, 1, 1, 0x3A);
    func_80046A2C(ot, 0xE0, 0x72, 0x14, 0x10, 0x14, 0xB4, 0, 0, 0, 0x244, 1, 1, 0x3A);

    xTest = (u8)x;
    zero = 0;
    if (xTest != 0xB8) {
        s32 green = alpha & 0xFF;

        func_8004711C(ot, (s16)((u8)x - 1), 0x4C, (s16)((u8)x - 1), 0x84, zero, green, zero, 0xFF);
        func_8004711C(ot, (s16)((u8)x - 3), 0x60, (s16)((u8)x - 3), 0x68, zero, green, zero, 0xFF);
        func_8004711C(ot, (s16)((u8)x - 5), 0x60, (s16)((u8)x - 5), 0x68, zero, green, zero, 0xFF);
        func_8004711C(ot, (s16)((u8)x - 7), 0x60, (s16)((u8)x - 7), 0x68, zero, green, zero, 0xFF);
        func_80046BA0(ot, (s16)((u8)x - 13), 0x64, (s16)((u8)x - 8), 0x5E, (s16)((u8)x - 8), 0x6A,
                      zero, green, zero, zero, 0x80);
    }

    if (xTest != 0xF7) {
        s32 green = alpha & 0xFF;

        func_8004711C(ot, (s16)((u8)x + 1), 0x4C, (s16)((u8)x + 1), 0x84, zero, green, zero, 0xFF);
        func_8004711C(ot, (s16)((u8)x + 3), 0x6A, (s16)((u8)x + 3), 0x72, zero, green, zero, 0xFF);
        func_8004711C(ot, (s16)((u8)x + 5), 0x6A, (s16)((u8)x + 5), 0x72, zero, green, zero, 0xFF);
        func_8004711C(ot, (s16)((u8)x + 7), 0x6A, (s16)((u8)x + 7), 0x72, zero, green, zero, 0xFF);
        func_80046BA0(ot, (s16)((u8)x + 14), 0x6E, (s16)((u8)x + 9), 0x69, (s16)((u8)x + 9), 0x73,
                      zero, green, zero, zero, 0x80);
    }

    func_80047460(ot, 0xB8, 0x48, 0x40, 0x40, gray, gray, gray, 0xFF);
    yLarge = 0x85;
    ySmall = 0x60;
    func_8004711C(ot, 0xC7, 0x4A, 0xC7, yLarge, gray, gray, gray, ySmall);
    func_8004711C(ot, 0xD7, 0x4A, 0xD7, yLarge, gray, gray, gray, ySmall);
    func_8004711C(ot, 0xE7, 0x4A, 0xE7, yLarge, gray, gray, gray, ySmall);

    func_80046BA0(ot, 0xB9, 0x87, 0xF7, 0x48, 0xF7, 0x87, 0x1E, 0x8E, 0x95, zero, 0x80);
    func_80047024(ot, 0xB8, 0x48, 0x40, 0x40, 0x95, 0x25, 0x1E, 0xFF);

    D_8009B268 += 0x60;
}

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
    if (wide != 0 || g_MenuAltLayout != 0) {
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

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80048ED8", func_800496F0);
