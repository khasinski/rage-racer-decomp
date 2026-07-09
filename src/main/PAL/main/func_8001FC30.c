#include "common.h"

void *func_8001720C(
    void *ot, void *prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b);
void *func_80017390(void *ot, void *prim, s32 arg2);

void func_8001FC30(s32 x, s32 y) {
    void *ot;
    register void *prim asm("$6");
    s32 redStack;
    register s32 green asm("$8");
    register s32 temp asm("$3");
    register s32 quotient asm("$2");
    register s32 red asm("$5");
    s32 blue;
    s32 width;
    s32 height;

    ot = *(u8 **)((u8 *)0x801A0000 - 0x3700) + 0xCC;
    prim = *(void **)0x1F800000;

    redStack = y;
    /* Match note: make GCC materialize the red clamp from a3 into a1. */
    __asm__("" : "=r"(redStack) : "0"(redStack));
    temp = x;
    if (x < 0) {
        temp = x + 7;
    }
    quotient = temp >> 3;
    green = quotient + redStack;
    if (x < 0) {
        x += 3;
    }
    blue = (x >> 2) + redStack;

    if (redStack < 0) {
        goto red_zero;
    }
    red = redStack;
    if (red < 0x100) {
        goto red_done;
    }
    red = 0xFF;
    goto red_done;
red_zero:
    red = 0;
red_done:
    redStack = red;

    if (green < 0) {
        goto green_zero;
    }
    temp = green;
    if (temp < 0x100) {
        goto green_done;
    }
    temp = 0xFF;
    goto green_done;
green_zero:
    temp = 0;
green_done:
    green = temp;

    if (blue < 0) {
        goto blue_zero;
    }
    temp = blue;
    if (temp < 0x100) {
        goto blue_done;
    }
    temp = 0xFF;
    goto blue_done;
blue_zero:
    temp = 0;
blue_done:

    width = 0x140;
    height = 0xF0;
    prim = func_8001720C(ot, prim, 0, 0, width, height, redStack, green, temp);
    *(void **)0x1F800000 = func_80017390(ot, prim, 0x49);
}
