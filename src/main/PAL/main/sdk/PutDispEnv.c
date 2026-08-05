#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

typedef struct Env {
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

typedef struct Cache {
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

extern GfxState g_GpuFuncs;
extern char D_80013614[];
extern Cache g_DispEnvCache;

long get_dx(void *arg0);
void *MemCopy(void *dst, void *src, long count);

/* libgpu PutDispEnv: GP1(05h/06h/07h/08h) from a 0x14-byte DISPENV.
 * Named from its own trace string D_80013614, "PutDispEnv(%08x)...". */
Env *PutDispEnv(Env *env);
Env *PutDispEnv(Env *arg0) {
    Env *s0 = arg0;
    long flags;
    u_long cmd;

    flags = 0x8000000;

    if (g_GpuFuncs.graphDebug >= 2) {
        g_GpuFuncs.printf(D_80013614, s0);
    }

    if (g_GpuFuncs.graphType == 1 || g_GpuFuncs.graphType == 2) {
        long r = get_dx(s0);
        cmd = (((u_short)s0->x2 & 0xfff) << 12 | (r & 0xfff)) | 0x5000000;
    } else {
        cmd = (((u_short)s0->x2 & 0x3ff) << 10 | ((u_short)s0->x0 & 0x3ff)) | 0x5000000;
    }
    g_GpuFuncs.funcs->submit(cmd);

    if ((short)g_DispEnvCache.x8 != s0->x8 || (short)g_DispEnvCache.xA != s0->xA ||
        (short)g_DispEnvCache.xC != s0->xC || (short)g_DispEnvCache.xE != s0->xE) {
        long left;
        long right;
        long top;
        long bottom;
        long xa;

        s0->x12 = GetDMAInterruptState();
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
            long t;
            t = left < 500 ? 500 : (left > 3290 ? 3290 : left);
            left = t;
        }
        {
            long t;
            t = right < left + 80 ? left + 80 : (right > 3290 ? 3290 : right);
            right = t;
        }
        {
            long t;
            t = top < 16 ? 16
                : (top > (s0->x12 ? 310 : 256) ? (s0->x12 ? 310 : 256) : top);
            top = t;
        }
        {
            long t;
            t = bottom < top + 2 ? top + 2
                : (bottom > (s0->x12 ? 312 : 258) ? (s0->x12 ? 312 : 258) : bottom);
            bottom = t;
        }
        {
            long hi = (right & 0xfff) << 12;
            long lo = left & 0xfff;
            u_long k = 0x6000000;
            g_GpuFuncs.funcs->submit(hi | (lo | k));
        }
        {
            long hi = (bottom & 0x3ff) << 10;
            long lo = top & 0x3ff;
            u_long k = 0x7000000;
            g_GpuFuncs.funcs->submit(hi | (lo | k));
        }
    }

    if (g_DispEnvCache.x10 != *(u_long *)&s0->x10 || (short)g_DispEnvCache.x0 != s0->x0 ||
        (short)g_DispEnvCache.x2 != s0->x2 || (short)g_DispEnvCache.x4 != s0->x4 ||
        (short)g_DispEnvCache.x6 != s0->x6) {
        s0->x12 = GetDMAInterruptState();
        if (s0->x12 == 1) {
            flags |= 8;
        }
        if (s0->x11 != 0) {
            flags |= 0x10;
        }
        if (s0->x10 != 0) {
            flags |= 0x20;
        }
        if (g_GpuFuncs.graphReverse != 0) {
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
        g_GpuFuncs.funcs->submit(flags);
    }

    MemCopy(&g_DispEnvCache, s0, 0x14);
    return s0;
}
