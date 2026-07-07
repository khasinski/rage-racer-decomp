#include "common.h"

extern u8 *D_8019C900;
extern s32 D_801E40B8;

void *func_80016F8C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void func_8002317C(void) {
    register u8 *base asm("$16");
    register s32 height asm("$17");
    register s32 clut asm("$18");
    register void **scratch asm("$19");
    register s32 fade asm("$20");
    register s32 value asm("$3");
    void *next;

    fade = D_801E40B8;
    if (fade >= 0) {
        value = fade;
        if (value >= 0x100) {
            value = 0xFF;
        }
    } else {
        value = 0;
    }
    fade = value;

    base = D_8019C900 + 0xCC;
    scratch = (void **)0x1F800000;

    next = func_80016F8C(base, *scratch, 0x64, 0xEC, 0x7C, 0x18, 0x80, 0, 0x3F97, fade);

    height = 0x20;
    clut = 0x3FD7;
    next = func_80016F8C(base, next, 0xDC, 0xC4, 8, 0x10, 0, height, clut, fade);
    next = func_80016F8C(base, next, 0x64, 0xC4, 0x78, height, 0, 0, clut, fade);
    *scratch = func_80017390(base, next, 5);
}
