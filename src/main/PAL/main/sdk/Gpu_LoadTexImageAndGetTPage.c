#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

u8 *SetDefDrawEnv(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void LoadImage(Rect *rect, void *data) asm("func_80065B24");
s32 func_80064BB4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 Gpu_LoadTexImageAndGetTPage(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) asm("func_800648EC");
s32 Gpu_LoadTexImageAndGetTPage(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Rect rect;
    s32 width;

    rect.x = arg3;
    rect.y = arg4;
    rect.h = arg6;

    if (arg1 == 1) {
        goto one;
    }
    if (arg1 < 2) {
        if (arg1 == 0) {
            goto zero;
        }
        goto call;
    }
    if (arg1 == 2) {
        goto two;
    }
    goto call;

zero:
    width = arg5;
    if (width < 0) {
        width += 3;
    }
    rect.w = width >> 2;
    goto call;

one:
    width = arg5;
    width += (u32)arg5 >> 31;
    rect.w = width >> 1;
    goto call;

two:
    rect.w = arg5;

call:
    LoadImage(&rect, arg0);
    return func_80064BB4(arg1, arg2, arg3, arg4) & 0xFFFF;
}

s32 func_80064C7C(s32 arg0, s32 arg1);

s32 LoadClut2(void *arg0, s32 arg1, s32 arg2) asm("func_800649D8");
s32 LoadClut2(void *arg0, s32 arg1, s32 arg2) {
    Rect rect;

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x100;
    rect.h = 1;

    LoadImage(&rect, arg0);
    return func_80064C7C(arg1, arg2) & 0xFFFF;
}

s32 LoadClut(void *arg0, s32 arg1, s32 arg2) asm("func_80064A40");
s32 LoadClut(void *arg0, s32 arg1, s32 arg2) {
    Rect rect;

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x10;
    rect.h = 1;

    LoadImage(&rect, arg0);
    return func_80064C7C(arg1, arg2) & 0xFFFF;
}

s32 func_800657E4(void);

void *func_80064AA8(u8 *env, s32 x, s32 y, s32 w, s32 h) {
    register u8 *envReg asm("$17") = env;
    register s32 xReg asm("$19") = x;
    register s32 yReg asm("$20") = y;
    register s32 wReg asm("$16") = w;
    register s32 hReg asm("$18") = h;
    register s32 cmp asm("$2");
    s32 dmaState;
    s32 graphType;

    dmaState = GetDMAInterruptState();
    *(s16 *)&envReg[0x0] = xReg;
    *(s16 *)&envReg[0x2] = yReg;
    *(s16 *)&envReg[0x4] = wReg;
    *(s16 *)&envReg[0xC] = 0;
    *(s16 *)&envReg[0xE] = 0;
    *(s16 *)&envReg[0x10] = 0;
    *(s16 *)&envReg[0x12] = 0;
    envReg[0x19] = 0;
    envReg[0x1A] = 0;
    envReg[0x1B] = 0;
    envReg[0x16] = 1;
    *(s16 *)&envReg[0x6] = hReg;
    if (dmaState != 0) {
        cmp = hReg < 0x121;
    } else {
        cmp = hReg < 0x101;
    }
    envReg[0x17] = cmp;
    *(s16 *)&envReg[0x8] = xReg;
    *(s16 *)&envReg[0xA] = yReg;

    graphType = func_800657E4();
    if (graphType != 1) {
        func_800657E4();
    }

    *(s16 *)&envReg[0x14] = 10;
    envReg[0x18] = 0;
    return envReg;
}

u8 * SetDefDrawEnv(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_80064B78");
u8 *SetDefDrawEnv(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 *ret;

    ret = arg0;
    *(s16 *)&ret[0] = arg1;
    *(s16 *)&ret[2] = arg2;
    *(s16 *)&ret[4] = arg3;
    *(s16 *)&ret[8] = 0;
    *(s16 *)&ret[0xA] = 0;
    *(s16 *)&ret[0xC] = 0;
    *(s16 *)&ret[0xE] = 0;
    ret[0x11] = 0;
    ret[0x10] = 0;
    ret[0x13] = 0;
    ret[0x12] = 0;
    *(s16 *)&ret[6] = arg4;
    return ret;
}

s32 GetTPage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80064BB4");
s32 GetTPage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 mode;
    s32 value;

    mode = func_800657E4();
    if (mode == 1) {
        goto high_mode;
    }

    mode = func_800657E4();
    if (mode != 2) {
        goto low_mode;
    }

high_mode:
    value = ((arg0 & 3) << 9) | ((arg1 & 3) << 7) | ((arg3 & 0x300) >> 3);
    return value | ((arg2 & 0x3FF) >> 6);

low_mode:
    value = ((arg0 & 3) << 7) | ((arg1 & 3) << 5) | ((arg3 & 0x100) >> 4) | ((arg2 & 0x3FF) >> 6);
    return value | ((arg3 & 0x200) << 2);
}

s32 GetClut(s32 arg0, s32 arg1) asm("func_80064C7C");
s32 GetClut(s32 arg0, s32 arg1) {
    return ((arg1 << 6) | ((arg0 >> 4) & 0x3F)) & 0xFFFF;
}

extern char D_80013374[];
extern void (*D_800941E4)(char *, ...);

void DumpTPage(s32 arg0) asm("func_80064C94");
void DumpTPage(s32 arg0) {
    s32 mode;
    u32 value;

    mode = func_800657E4();
    if (mode == 1) {
        goto high_mode;
    }

    mode = func_800657E4();
    if (mode != 2) {
        goto low_mode;
    }

high_mode:
    D_800941E4(D_80013374, (((u32)arg0 & 0xFFFF) >> 9) & 3, (((u32)arg0 & 0xFFFF) >> 7) & 3, (((u32)arg0 & 0xFFFF) << 6) & 0x7C0, (((u32)arg0 & 0xFFFF) << 3) & 0x300);
    return;

low_mode:
    value = arg0 & 0xFFFF;
    D_800941E4(D_80013374, (value >> 7) & 3, (value >> 5) & 3, (value << 6) & 0x7C0, ((value << 4) & 0x100) + ((value >> 2) & 0x200));
}
