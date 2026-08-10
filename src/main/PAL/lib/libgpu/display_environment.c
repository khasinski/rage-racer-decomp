#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "psyq/gpu_environment_internal.h"


/* libgpu PutDispEnv: GP1(05h/06h/07h/08h) from a 0x14-byte DISPENV.
 * Named from its own trace string g_FmtGpuPutDispEnv, "PutDispEnv(%08x)...". */
DispEnv *PutDispEnv(DispEnv *env) {
    DispEnv *s0 = env;
    long flags;
    u_long cmd;

    flags = 0x8000000;

    if (g_GpuFuncs.graphDebug >= 2) {
        g_GpuFuncs.printf(g_FmtGpuPutDispEnv, s0);
    }

    if (g_GpuFuncs.graphType == 1 || g_GpuFuncs.graphType == 2) {
        long r = get_dx(s0);
        cmd = (((u_short)s0->disp.y & 0xfff) << 12 | (r & 0xfff)) | 0x5000000;
    } else {
        cmd = (((u_short)s0->disp.y & 0x3ff) << 10 | ((u_short)s0->disp.x & 0x3ff)) | 0x5000000;
    }
    g_GpuFuncs.funcs->submit(cmd);

    if ((short)g_DispEnvCache.screenX != s0->screen.x || (short)g_DispEnvCache.screenY != s0->screen.y ||
        (short)g_DispEnvCache.screenWidth != s0->screen.w || (short)g_DispEnvCache.screenHeight != s0->screen.h) {
        long left;
        long right;
        long top;
        long bottom;
        long xa;

        s0->pad0 = GetDMAInterruptState();
        left = s0->screen.x * 10 + 608;
        xa = s0->screen.y;
        if (s0->pad0) {
            top = xa + 19;
        } else {
            top = xa + 16;
        }
        right = s0->screen.w != 0 ? left + s0->screen.w * 10 : left + 2560;
        if (s0->screen.h == 0) {
            bottom = top + 240;
        } else {
            bottom = top + s0->screen.h;
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
                : (top > (s0->pad0 ? 310 : 256) ? (s0->pad0 ? 310 : 256) : top);
            top = t;
        }
        {
            long t;
            t = bottom < top + 2 ? top + 2
                : (bottom > (s0->pad0 ? 312 : 258) ? (s0->pad0 ? 312 : 258) : bottom);
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

    if (g_DispEnvCache.flags != *(u_long *)&s0->isinter || (short)g_DispEnvCache.displayX != s0->disp.x ||
        (short)g_DispEnvCache.displayY != s0->disp.y || (short)g_DispEnvCache.displayWidth != s0->disp.w ||
        (short)g_DispEnvCache.displayHeight != s0->disp.h) {
        s0->pad0 = GetDMAInterruptState();
        if (s0->pad0 == 1) {
            flags |= 8;
        }
        if (s0->isrgb24 != 0) {
            flags |= 0x10;
        }
        if (s0->isinter != 0) {
            flags |= 0x20;
        }
        if (g_GpuFuncs.graphReverse != 0) {
            flags |= 0x80;
        }
        if (s0->disp.w < 281) {
        } else if (s0->disp.w < 353) {
            flags |= 1;
        } else if (s0->disp.w < 401) {
            flags |= 0x40;
        } else if (s0->disp.w < 561) {
            flags |= 2;
        } else {
            flags |= 3;
        }
        {
            long v = s0->disp.h;
            long t = s0->pad0 ? v < 289 : v < 257;
            if (!t) {
                flags |= 0x24;
            }
        }
        g_GpuFuncs.funcs->submit(flags);
    }

    MemCopy((u_char *)&g_DispEnvCache, (u_char *)s0, 0x14);
    return s0;
}
