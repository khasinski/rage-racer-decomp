#include "common.h"


INCLUDE_RODATA("asm/PAL/main/nonmatchings/main/func_800487D8", func_800487D8_rodata);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_800487D8", func_800487D8);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_800487D8", func_800489AC);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_800487D8", func_80048B88);

extern s32 D_8009B264;
extern s32 D_8009E694;

s32 func_80068568(s32 arg0);
void func_80047460(
    void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 arg5, s32 color, s32 arg7, s32 arg8);

void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash) {
    void *ot;
    s32 savedX0;
    s32 savedY0;
    s32 savedX1;
    s32 savedY1;
    s32 color;
    s32 white;
    register s32 counter asm("$2");
    register s32 phaseBase asm("$4");

    ot = *(void **)0x1F800004;
    savedX0 = x0;
    savedY0 = y0;
    savedX1 = x1;
    savedY1 = y1;
    if (useFlash != 0) {
        if (D_8009E694 & 2) {
            color = 0xFF;
        } else {
            color = 0x60;
        }
    } else {
        counter = D_8009B264;
        phaseBase = counter;
        if (counter < 0) {
            phaseBase = counter + 0xFFF;
        }
        phaseBase = (phaseBase >> 12) << 12;
        counter = func_80068568(counter - phaseBase);
        if (counter < 0) {
            counter += 0x3F;
        }
        color = (counter >> 6) - 0x41;
    }

    white = 0xFF;
    func_80047460(
        ot,
        (s16)(savedX0 - 1),
        (s16)(savedY0 - 2),
        (s16)(savedX1 + 2),
        (s16)(savedY1 + 4),
        0,
        color & 0xFF,
        0,
        white);
    func_80047460(
        ot, (s16)savedX0, (s16)savedY0, (s16)savedX1, (s16)(savedY1 + 0), 0, color & 0xFF, 0, white);
    D_8009B264 += 0x60;
}
