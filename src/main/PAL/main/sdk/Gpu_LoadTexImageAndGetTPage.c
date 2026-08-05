#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "game/render.h"

long Gpu_LoadTexImageAndGetTPage(void *arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) {
    Rect rect;
    long width;

    rect.x = arg3;
    rect.y = arg4;
    rect.h = arg6;

    switch (arg1) {
    case 0:
    width = arg5;
    if (width < 0) {
        width += 3;
    }
    rect.w = width >> 2;
    break;

    case 1:
    width = arg5;
    width += (u_long)arg5 >> 31;
    rect.w = width >> 1;
    break;

    case 2:
    rect.w = arg5;
    break;
    }

    LoadImage(&rect, arg0);
    return GetTPage(arg1, arg2, arg3, arg4) & 0xFFFF;
}

long LoadClut2(void *arg0, long arg1, long arg2) {
    Rect rect;

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x100;
    rect.h = 1;

    LoadImage(&rect, arg0);
    return GetClut(arg1, arg2) & 0xFFFF;
}

long LoadClut(void *arg0, long arg1, long arg2) {
    Rect rect;

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x10;
    rect.h = 1;

    LoadImage(&rect, arg0);
    return GetClut(arg1, arg2) & 0xFFFF;
}

/* Fills the 0x1C-byte DRAWENV head. */
void *SetDefDrawEnv(u_char *env, long x, long y, long w, long h) {
    u_char *envReg = env;
    long xReg = x;
    long yReg = y;
    register long wReg asm("$16") = w;
    long hReg = h;
    long cmp;
    long dmaState;
    long graphType;

    dmaState = GetDMAInterruptState();
    *(short *)&envReg[0x0] = xReg;
    *(short *)&envReg[0x2] = yReg;
    *(short *)&envReg[0x4] = wReg;
    *(short *)&envReg[0xC] = 0;
    *(short *)&envReg[0xE] = 0;
    *(short *)&envReg[0x10] = 0;
    *(short *)&envReg[0x12] = 0;
    envReg[0x19] = 0;
    envReg[0x1A] = 0;
    envReg[0x1B] = 0;
    envReg[0x16] = 1;
    *(short *)&envReg[0x6] = hReg;
    if (dmaState != 0) {
        cmp = hReg < 0x121;
    } else {
        cmp = hReg < 0x101;
    }
    envReg[0x17] = cmp;
    *(short *)&envReg[0x8] = xReg;
    *(short *)&envReg[0xA] = yReg;

    graphType = GetGraphType();
    if (graphType != 1) {
        GetGraphType();
    }

    *(short *)&envReg[0x14] = 10;
    envReg[0x18] = 0;
    return envReg;
}

u_char *SetDefDispEnv(u_char *arg0, long arg1, long arg2, long arg3, long arg4) {
    u_char *ret;

    ret = arg0;
    *(short *)&ret[0] = arg1;
    *(short *)&ret[2] = arg2;
    *(short *)&ret[4] = arg3;
    *(short *)&ret[8] = 0;
    *(short *)&ret[0xA] = 0;
    *(short *)&ret[0xC] = 0;
    *(short *)&ret[0xE] = 0;
    ret[0x11] = 0;
    ret[0x10] = 0;
    ret[0x13] = 0;
    ret[0x12] = 0;
    *(short *)&ret[6] = arg4;
    return ret;
}

long GetTPage(long arg0, long arg1, long arg2, long arg3) {
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

    value = ((arg0 & 3) << 9) | ((arg1 & 3) << 7) | ((arg3 & 0x300) >> 3);
    return value | ((arg2 & 0x3FF) >> 6);

    }
    value = ((arg0 & 3) << 7) | ((arg1 & 3) << 5) | ((arg3 & 0x100) >> 4) | ((arg2 & 0x3FF) >> 6);
    return value | ((arg3 & 0x200) << 2);
}

long GetClut(long arg0, long arg1) {
    return ((arg1 << 6) | ((arg0 >> 4) & 0x3F)) & 0xFFFF;
}

void DumpTPage(long arg0) {
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
    GPU_printf(D_80013374, (((u_long)arg0 & 0xFFFF) >> 9) & 3, (((u_long)arg0 & 0xFFFF) >> 7) & 3, (((u_long)arg0 & 0xFFFF) << 6) & 0x7C0, (((u_long)arg0 & 0xFFFF) << 3) & 0x300);
    return;

    }
    value = arg0 & 0xFFFF;
    GPU_printf(D_80013374, (value >> 7) & 3, (value >> 5) & 3, (value * 64) & 0x7C0, ((value * 16) & 0x100) + ((value >> 2) & 0x200));
}
