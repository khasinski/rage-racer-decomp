#include "common.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "psyq/gpu.h"

void func_800218A0(s32 arg0);
s32 func_80016EC4(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);

void func_80022FAC(void) {
    s32 v0, v1;
    if ((g_SceneTimer = g_SceneTimer + 1) == 2) {
        SetDispMask(1);
    }
    v0 = g_FadeLevel + g_FadeStep;
    g_FadeLevel = v0;
    v1 = g_FadeStep;
    if (v1 > 0) {
        if (v0 >= 257) {
            g_FadeLevel = 0x100;
            g_FadeStep = 0;
        }
    } else if (v1 == 0) {
        if (g_SceneTimer == 0x12C || (g_PadEdge2 & 0x860)) {
            g_FadeLevel = 0x100;
            g_FadeStep = -4;
        }
    } else if (v0 == 0) {
        g_SceneId = 2;
    }
    func_800218A0(g_FadeLevel);
}

void func_800230B0(void) {
    u8 *base;
    s32 clut;
    s32 height;
    volatile s32 *scratch;
    s32 next;

    base = g_DrawBuffer + 0xCC;
    height = 0xF0;
    clut = 0x3FDB;
    scratch = (volatile s32 *)0x1F800000;

    next = *scratch;
    next = func_80016EC4(base, next, 0, 0, 0x100, height, 0, 0, clut);
    next = func_80017390(base, next, 6);
    next = func_80016EC4(base, next, 0x100, 0, 0x40, height, 0, 0, clut);
    *scratch = func_80017390(base, next, 7);
}
