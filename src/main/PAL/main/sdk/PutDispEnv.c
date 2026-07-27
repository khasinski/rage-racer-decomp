#include "common.h"
#include "psyq/gpu.h"

typedef struct {
    short x0;
    short x2;
    short x4;
    short x6;
    short x8;
    short xA;
    short xC;
    short xE;
    u_char  x10;
    u_char  x11;
    u_char  x12;
    u_char  x13;
} Env;

typedef struct {
    volatile u_short x0;
    volatile u_short x2;
    volatile u_short x4;
    volatile u_short x6;
    volatile u_short x8;
    volatile u_short xA;
    volatile u_short xC;
    volatile u_short xE;
    u_long x10;
} Cache;

typedef struct {
    GpuCallbacks *funcs;
    void (*printf)(char *, ...);
    u_char e8;
    u_char e9;
    volatile u_char debug;
    volatile u_char eb;
} GfxState;

extern GfxState D_800941E0;
extern char D_80013614[];
extern Cache D_80094254;

long func_80066CB0(void *arg0);
long func_8006EAEC(void);
void *MemCopy(void *dst, void *src, long count) asm("func_800681BC");

/* libgpu PutDispEnv: GP1(05h/06h/07h/08h) from a 0x14-byte DISPENV.
 * Named from its own trace string D_80013614, "PutDispEnv(%08x)...". */
Env *PutDispEnv(Env *env) asm("func_800660AC");
Env *PutDispEnv(Env *arg0) {
    Env *s0 = arg0;
    long flags;
    u_long cmd;

    flags = 0x8000000;

    if (D_800941E0.debug >= 2) {
        D_800941E0.printf(D_80013614, s0);
    }

    if (D_800941E0.e8 == 1 || D_800941E0.e8 == 2) {
        long r = func_80066CB0(s0);
        cmd = (((u_short)s0->x2 & 0xfff) << 12 | (r & 0xfff)) | 0x5000000;
    } else {
        cmd = (((u_short)s0->x2 & 0x3ff) << 10 | ((u_short)s0->x0 & 0x3ff)) | 0x5000000;
    }
    D_800941E0.funcs->submit(cmd);

    if ((short)D_80094254.x8 != s0->x8 || (short)D_80094254.xA != s0->xA ||
        (short)D_80094254.xC != s0->xC || (short)D_80094254.xE != s0->xE) {
        long left;
        long right;
        long top;
        long bottom;
        long xa;

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
            register long t asm("$4");
            t = left < 500 ? 500 : (left > 3290 ? 3290 : left);
            left = t;
        }
        {
            register long t asm("$3");
            t = right < left + 80 ? left + 80 : (right > 3290 ? 3290 : right);
            right = t;
        }
        {
            register long t asm("$4");
            t = top < 16 ? 16
                : (top > (s0->x12 ? 310 : 256) ? (s0->x12 ? 310 : 256) : top);
            top = t;
        }
        {
            register long t asm("$3");
            t = bottom < top + 2 ? top + 2
                : (bottom > (s0->x12 ? 312 : 258) ? (s0->x12 ? 312 : 258) : bottom);
            bottom = t;
        }
        {
            register long hi asm("$2") = (right & 0xfff) << 12;
            long lo = left & 0xfff;
            register u_long k asm("$3") = 0x6000000;
            asm("" : "=r"(lo) : "0"(lo));
            D_800941E0.funcs->submit(hi | (lo | k));
        }
        {
            register long hi asm("$2") = (bottom & 0x3ff) << 10;
            long lo = top & 0x3ff;
            register u_long k asm("$3") = 0x7000000;
            asm("" : "=r"(lo) : "0"(lo));
            D_800941E0.funcs->submit(hi | (lo | k));
        }
    }

    if (D_80094254.x10 != *(u_long *)&s0->x10 || (short)D_80094254.x0 != s0->x0 ||
        (short)D_80094254.x2 != s0->x2 || (short)D_80094254.x4 != s0->x4 ||
        (short)D_80094254.x6 != s0->x6) {
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
            long v = s0->x6;
            long t = s0->x12 ? v < 289 : v < 257;
            if (!t) {
                flags |= 0x24;
            }
        }
        D_800941E0.funcs->submit(flags);
    }

    MemCopy(&D_80094254, s0, 0x14);
    return s0;
}
