#include "common.h"

extern s32 D_801E6F1C;
extern s32 D_801E4DA8;
extern u8 *D_8019C900;

void *func_80016F8C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void *func_80017390(void *arg0, void *arg1, s32 arg2);
void *func_800173F4(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);

void func_8001B974(void) {
    register s32 alpha asm("$18");
    void *base;
    register s32 color asm("$20");
    register s32 h88 asm("$21");
    register s32 scratch asm("$22");
    register s32 hF0 asm("$16");
    register s32 clut0 asm("$17");
    void *next;
    register s32 tmp asm("$2");
    register s32 clamped asm("$3");
    register s32 x28 asm("$6");
    register s32 yA0 asm("$7");

    tmp = D_801E6F1C;
    tmp <<= 1;
    clamped = 0x7F;
    alpha = clamped - tmp;
    if (alpha < 0x30) {
        goto low_alpha;
    }
    clamped = alpha;
    if (clamped < 0x80) {
        goto alpha_done;
    }
    clamped = 0x7F;
    goto alpha_done;
low_alpha:
    clamped = 0x30;
alpha_done:
    alpha = clamped;

    x28 = 0x28;
    yA0 = 0xA0;
    asm("" : "=r"(x28), "=r"(yA0) : "0"(x28), "1"(yA0)); /* Match note: materialize first-call argument registers before the stack-arg temp. */
    color = 0x7E00;
    scratch = 0x1F800000;
    hF0 = 0xF0;
    asm("" : "=r"(scratch), "=r"(hF0) : "0"(scratch), "1"(hF0)); /* Match note: keep scratchpad base and 0xf0 materialized before the first stack-arg temp. */
    tmp = 0x18;
    next = *(void **)scratch;
    h88 = 0x88;
    clut0 = 0x7DC0;

    next = func_80016F8C(base = D_8019C900 + 0xD0, next, x28, yA0, hF0, tmp, 0, h88, clut0, alpha);
    next = func_80016F8C(base, next, 0x20, 0xB8, 0x100, 0x10, 0, hF0, 0x7DC1, alpha);
    next = func_80016F8C(base, next, 0x11A, 0xAF, 0xC, 8, 0xE0, 0xB0, clut0, alpha);
    next = func_80017390(base, next, 0x19);

    if (D_801E4DA8 >= 0xB) {
        color = 0x7D80;
    }

    next = func_80016F8C(base, next, 0x34, 0x18, 0x6C, h88, 0, 0, color, alpha);
    *(void **)scratch = func_800173F4(base, next, 0xA0, 0x18, -0x6C, h88, 0, 0, color, 0x99, alpha);
}
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8001B974", func_8001BB58);
