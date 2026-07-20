#include "common.h"
#include "psyq/gpu.h"

typedef struct {
    s16 x0;
    s16 x2;
    s16 x4;
    s16 x6;
    s16 x8;
    s16 xA;
    s16 xC;
    s16 xE;
    u8  x10;
    u8  x11;
    u8  x12;
    u8  x13;
} Env;

typedef struct {
    volatile u16 x0;
    volatile u16 x2;
    volatile u16 x4;
    volatile u16 x6;
    volatile u16 x8;
    volatile u16 xA;
    volatile u16 xC;
    volatile u16 xE;
    u32 x10;
} Cache;

typedef struct {
    GpuCallbacks *funcs;
    void (*printf)(char *, ...);
    u8 e8;
    u8 e9;
    volatile u8 debug;
    volatile u8 eb;
} GfxState;

extern GfxState D_800941E0;
extern char D_80013614[];
extern Cache D_80094254;

s32 func_80066CB0(void *arg0);
s32 func_8006EAEC(void);
void *func_800681BC(void *dst, void *src, s32 count);

Env *func_800660AC(Env *arg0) {
    Env *s0 = arg0;
    s32 flags;
    u32 cmd;

    flags = 0x8000000;

    if (D_800941E0.debug >= 2) {
        D_800941E0.printf(D_80013614, s0);
    }

    if (D_800941E0.e8 == 1 || D_800941E0.e8 == 2) {
        s32 r = func_80066CB0(s0);
        cmd = (((u16)s0->x2 & 0xfff) << 12 | (r & 0xfff)) | 0x5000000;
    } else {
        cmd = (((u16)s0->x2 & 0x3ff) << 10 | ((u16)s0->x0 & 0x3ff)) | 0x5000000;
    }
    D_800941E0.funcs->submit(cmd);

    if ((s16)D_80094254.x8 != s0->x8 || (s16)D_80094254.xA != s0->xA ||
        (s16)D_80094254.xC != s0->xC || (s16)D_80094254.xE != s0->xE) {
        s32 left;
        s32 right;
        s32 top;
        s32 bottom;
        s32 xa;

        s0->x12 = func_8006EAEC();
        left = s0->x8 * 10 + 608;
        xa = s0->xA;
        if (s0->x12) {
            top = xa + 19;
        } else {
            top = xa + 16;
        }
        right = s0->xC != 0 ? left + s0->xC * 10 : left + 2560;
        if (s0->xE == 0) {
            bottom = top + 240;
        } else {
            bottom = top + s0->xE;
        }
        {
            register s32 t asm("$4");
            t = left < 500 ? 500 : (left > 3290 ? 3290 : left);
            left = t;
        }
        {
            register s32 t asm("$3");
            t = right < left + 80 ? left + 80 : (right > 3290 ? 3290 : right);
            right = t;
        }
        {
            register s32 t asm("$4");
            t = top < 16 ? 16
                : (top > (s0->x12 ? 310 : 256) ? (s0->x12 ? 310 : 256) : top);
            top = t;
        }
        {
            register s32 t asm("$3");
            t = bottom < top + 2 ? top + 2
                : (bottom > (s0->x12 ? 312 : 258) ? (s0->x12 ? 312 : 258) : bottom);
            bottom = t;
        }
        {
            register s32 hi asm("$2") = (right & 0xfff) << 12;
            s32 lo = left & 0xfff;
            register u32 k asm("$3") = 0x6000000;
            asm("" : "=r"(lo) : "0"(lo));
            D_800941E0.funcs->submit(hi | (lo | k));
        }
        {
            register s32 hi asm("$2") = (bottom & 0x3ff) << 10;
            s32 lo = top & 0x3ff;
            register u32 k asm("$3") = 0x7000000;
            asm("" : "=r"(lo) : "0"(lo));
            D_800941E0.funcs->submit(hi | (lo | k));
        }
    }

    if (D_80094254.x10 != *(u32 *)&s0->x10 || (s16)D_80094254.x0 != s0->x0 ||
        (s16)D_80094254.x2 != s0->x2 || (s16)D_80094254.x4 != s0->x4 ||
        (s16)D_80094254.x6 != s0->x6) {
        s0->x12 = func_8006EAEC();
        if (s0->x12 == 1) {
            flags |= 8;
        }
        if (s0->x11 != 0) {
            flags |= 0x10;
        }
        if (s0->x10 != 0) {
            flags |= 0x20;
        }
        if (D_800941E0.eb != 0) {
            flags |= 0x80;
        }
        if (s0->x4 < 281) {
        } else if (s0->x4 < 353) {
            flags |= 1;
        } else if (s0->x4 < 401) {
            flags |= 0x40;
        } else if (s0->x4 < 561) {
            flags |= 2;
        } else {
            flags |= 3;
        }
        {
            s32 v = s0->x6;
            s32 t = s0->x12 ? v < 289 : v < 257;
            if (!t) {
                flags |= 0x24;
            }
        }
        D_800941E0.funcs->submit(flags);
    }

    func_800681BC(&D_80094254, s0, 0x14);
    return s0;
}
