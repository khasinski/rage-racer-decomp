#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "game/render.h"

long Gpu_LoadTexImageAndGetTPage(void *pixels, long mode, long x, long y, long w, long h, long abr) {
    Rect rect;
    long width;

    rect.x = y;
    rect.y = w;
    rect.h = abr;

    switch (mode) {
    case 0:
    width = h;
    if (width < 0) {
        width += 3;
    }
    rect.w = width >> 2;
    break;

    case 1:
    width = h;
    width += (u_long)h >> 31;
    rect.w = width >> 1;
    break;

    case 2:
    rect.w = h;
    break;
    }

    LoadImage(&rect, pixels);
    return GetTPage(mode, x, y, w) & 0xFFFF;
}

long LoadClut2(void *clut, long x, long y) {
    Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = 0x100;
    rect.h = 1;

    LoadImage(&rect, clut);
    return GetClut(x, y) & 0xFFFF;
}

long LoadClut(void *clut, long x, long y) {
    Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = 0x10;
    rect.h = 1;

    LoadImage(&rect, clut);
    return GetClut(x, y) & 0xFFFF;
}

/* Fills the 0x1C-byte DRAWENV head. */
DrawEnv *SetDefDrawEnv(DrawEnv *env, long x, long y, long w, long h) {
    long dmaState;
    long graphType;

    dmaState = GetDMAInterruptState();
    env->clip.x = x;
    env->clip.y = y;
    env->clip.w = w;
    env->tw.x = 0;
    env->tw.y = 0;
    env->tw.w = 0;
    env->tw.h = 0;
    env->r0 = 0;
    env->g0 = 0;
    env->b0 = 0;
    env->dtd = 1;
    env->clip.h = h;
    if (dmaState != 0) {
        env->dfe = h < 0x121;
    } else {
        env->dfe = h < 0x101;
    }
    env->ofs[0] = x;
    env->ofs[1] = y;

    graphType = GetGraphType();
    if (graphType != 1) {
        GetGraphType();
    }

    env->tpage = 10;
    env->isbg = 0;
    return env;
}

u_char *SetDefDispEnv(u_char *env, long x, long y, long w, long h) {
    u_char *ret;

    ret = env;
    *(short *)&ret[0] = x;
    *(short *)&ret[2] = y;
    *(short *)&ret[4] = w;
    *(short *)&ret[8] = 0;
    *(short *)&ret[0xA] = 0;
    *(short *)&ret[0xC] = 0;
    *(short *)&ret[0xE] = 0;
    ret[0x11] = 0;
    ret[0x10] = 0;
    ret[0x13] = 0;
    ret[0x12] = 0;
    *(short *)&ret[6] = h;
    return ret;
}

long GetTPage(long tp, long abr, long x, long y) {
    long mode;
    long value;

    mode = GetGraphType();
    switch (0) { default:
    if (!(mode == 1)) {

    mode = GetGraphType();
    if (mode != 2) {
        break;
    }
    }

    value = ((tp & 3) << 9) | ((abr & 3) << 7) | ((y & 0x300) >> 3);
    return value | ((x & 0x3FF) >> 6);

    }
    value = ((tp & 3) << 7) | ((abr & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6);
    return value | ((y & 0x200) << 2);
}

long GetClut(long tp, long abr) {
    return ((abr << 6) | ((tp >> 4) & 0x3F)) & 0xFFFF;
}

void DumpTPage(long x) {
    long mode;
    u_long value;

    mode = GetGraphType();
    switch (0) { default:
    if (mode == 1) {
    } else {

    mode = GetGraphType();
    if (mode != 2) {
        break;
    }

    }
    GPU_printf(g_FmtGpuTPage, (((u_long)x & 0xFFFF) >> 9) & 3, (((u_long)x & 0xFFFF) >> 7) & 3, (((u_long)x & 0xFFFF) << 6) & 0x7C0, (((u_long)x & 0xFFFF) << 3) & 0x300);
    return;

    }
    value = x & 0xFFFF;
    GPU_printf(g_FmtGpuTPage, (value >> 7) & 3, (value >> 5) & 3, (value * 64) & 0x7C0, ((value * 16) & 0x100) + ((value >> 2) & 0x200));
}
