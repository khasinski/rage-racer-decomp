#include "common.h"

typedef unsigned int u_long;

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} RECT;

typedef struct {
    RECT disp;
} DISPENV;

extern u8 g_GraphType asm("D_800941E8");
extern u8 g_GraphReverse asm("D_800941EB");

u_long get_dx(DISPENV *env) asm("func_80066CB0");

u_long get_dx(DISPENV *env) {
    register volatile u8 *modep asm("$2") = &g_GraphType;
    register s32 value asm("$2");
    register s32 mode asm("$3");

    value = *modep;
    asm("" : "=r"(value) : "0"(value));
    mode = value & 0xFF;

    switch (mode) {
    case 1:
        return g_GraphReverse ? 0x400 - env->disp.x - env->disp.w : env->disp.x;
    case 2:
        return g_GraphReverse ? 0x400 - env->disp.x - (env->disp.w / 2)
                          : env->disp.x / 2;
    default:
        return env->disp.x;
    }
}
