#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

s32 func_800657E4(void);

void SetDrawTPage(u_char *arg0, long arg1, long arg2, long arg3);
void SetDrawTPage(u_char *arg0, long arg1, long arg2, long arg3) {
s32 encoded;
    s32 mode;

    arg0[3] = 1;
    mode = func_800657E4();
    if ((mode == 1) || (func_800657E4() == 2)) {
        s32 value = 0xE1000000;

        if (arg2 != 0) {
            value = 0xE1000800;
        }
        encoded = arg3 & 0x27FF;
        if (arg1 != 0) {
            encoded |= 0x1000;
        }
        { s32 rel = encoded; encoded = value | rel; }
    } else {
        register s32 value asm("$7") = 0xE1000000;

        if (arg2 != 0) {
            value = 0xE1000200;
        }
        encoded = arg3 & 0x9FF;
        if (arg1 != 0) {
            encoded |= 0x400;
        }
        { s32 rel = encoded; encoded = value | rel; }
    }
    *(u32 *)&arg0[4] = encoded;
}

void SetDrawLoad(u8 *arg0, u8 *arg1);
void SetDrawLoad(u8 *arg0, u8 *arg1) {
    s32 sign;
    s32 value;
    s32 size;

    sign = *(s16 *)&arg1[4];
    value = sign * *(s16 *)&arg1[6];
    value += 1;
    sign = (u32)value >> 31;
    value += sign;
    value >>= 1;
    size = value + 4;
    if ((u32)(value - 1) >= 0xB) {
        size = 0;
    }

    *(u32 *)&arg0[4] = 0x01000000;
    arg0[3] = size;
    *(u32 *)&arg0[8] = 0xA0000000;
    *(u32 *)&arg0[0xC] = *(u32 *)&arg1[0];
    *(u32 *)&arg0[0x10] = *(u32 *)&arg1[4];
}

s32 MargePrim(u8 *arg0, u8 *arg1);
s32 MargePrim(u8 *arg0, u8 *arg1) {
    s32 value;

    value = arg0[3] + arg1[3] + 1;
    if (value >= 0x21) {
        return -1;
    }
    arg0[3] = value;
    *(u32 *)arg1 = 0;
    return 0;
}

extern char D_80013374[];
extern char D_8001339C[];
extern char D_800133B4[];
extern char D_800133C4[];
extern char D_800133DC[];
extern char D_800133E8[];
extern char D_800133F4[];
extern char D_80013410[];
extern char D_8001342C[];
extern char D_80013438[];
extern void (*D_800941E4)(char *, ...);

void DumpDrawEnv(DrawEnv *arg0);
void DumpDrawEnv(DrawEnv *arg0) {
    s32 mode;
    u32 value;

    D_800941E4(D_8001339C, arg0->clip.x, arg0->clip.y, arg0->clip.w, arg0->clip.h);
    D_800941E4(D_800133B4, arg0->ofs[0], arg0->ofs[1]);
    D_800941E4(D_800133C4, arg0->tw.x, arg0->tw.y, arg0->tw.w, arg0->tw.h);
    D_800941E4(D_800133DC, arg0->dtd);
    D_800941E4(D_800133E8, arg0->dfe);

    mode = func_800657E4();
    switch (0) { default:
    if (mode == 1) {
    } else {

    mode = func_800657E4();
    if (mode != 2) {
        break;
    }

    }
    value = arg0->tpage;
    D_800941E4(D_80013374, (value >> 9) & 3, (value >> 7) & 3, (value * 64) & 0x7C0, (value * 8) & 0x300);
    return;

    }
    value = arg0->tpage;
    D_800941E4(D_80013374, (value >> 7) & 3, (value >> 5) & 3, (value * 64) & 0x7C0, ((value * 16) & 0x100) + ((value >> 2) & 0x200));
}

void DumpDispEnv(DispEnv *arg0);
void DumpDispEnv(DispEnv *arg0) {
    D_800941E4(D_800133F4, arg0->disp.x, arg0->disp.y, arg0->disp.w, arg0->disp.h);
    D_800941E4(D_80013410, arg0->screen.x, arg0->screen.y, arg0->screen.w, arg0->screen.h);
    D_800941E4(D_8001342C, arg0->isinter);
    D_800941E4(D_80013438, arg0->isrgb24);
}

extern char D_80013478[];
extern char D_80013498[];
extern u8 g_GpuJumpTable[] asm("D_800941A0");
extern GpuCallbacks *g_GpuFuncs asm("D_800941E0");
extern u8 g_GraphType[] asm("D_800941E8");
extern u8 g_GraphQueue asm("D_800941E9");
extern u8 g_GraphDebug asm("D_800941EA");
extern u16 g_VramWidth asm("D_800941EC");
extern u16 g_VramHeight asm("D_800941EE");
extern u8 g_VramWidthTable[] asm("D_80094268");
extern u8 g_VramHeightTable[] asm("D_8009427C");

void func_80068180(u8 *dst, s32 value, s32 count);
void GPU_cw(void *arg0) asm("func_800681AC");
s32 Gpu_Reset(s32 arg0) asm("func_80067C80");

/* libgpu ResetGraph. Own trace strings D_80013478 "ResetGraph:jtb=%08x,env=%08x"
 * and D_80013498 "ResetGraph(%d)..."; mode&7 of 0 or 3 does the full reset. */
void ResetGraph(s32 mode) asm("func_80065460");
void ResetGraph(s32 mode) {
    s32 maskedMode;
    u8 *graphState;
    s32 graphType;
    u8 *clearEnv;
    s32 fillValue;

    maskedMode = mode & 7;
    if ((maskedMode == 0) || (maskedMode == 3)) {
        graphState = g_GraphType;
        DebugPrintf(D_80013478, g_GpuJumpTable, graphState);
        func_80068180(graphState, 0, 0x80);
        KernelCallbackSlot3();
        GPU_cw((void *)((u32)g_GpuFuncs & 0xFFFFFF));
        graphType = Gpu_Reset(maskedMode != 0);
        clearEnv = graphState + 0x10;
        *(volatile u8 *)graphState = graphType;
        {
            s32 st0 = *(volatile u8 *)graphState;
            u16 v;
            s32 st1;

            g_GraphQueue = 1;
            v = *(u16 *)&g_VramWidthTable[st0 * 4];
            st1 = *(volatile u8 *)graphState;
            fillValue = -1;
            g_VramWidth = v;
            g_VramHeight = *(u16 *)&g_VramHeightTable[st1 * 4];
        }
        func_80068180(clearEnv, fillValue, 0x5C);
        func_80068180(graphState + 0x6C, -1, 0x14);
        graphType = *(volatile u8 *)graphState;
    } else {
        if (g_GraphDebug >= 2) {
            D_800941E4(D_80013498, mode);
        }
        g_GpuFuncs->resetGraph(1);
    }
}

extern void (* volatile GPU_printf)(char *, ...) asm("D_800941E4");
extern GpuCallbacks *g_GpuFuncs asm("D_800941E0");
extern u8 g_GraphDebug asm("D_800941EA");
extern u8 g_GraphReverse asm("D_800941EB");
extern char D_800134AC[];
extern char D_800134C4[];

s32 SetGraphReverse(s32 arg0);

s32 SetGraphReverse(s32 arg0) {
    u8 *state = &g_GraphReverse;
    s32 newValue = arg0;
    s32 old = *state;
    GpuCallbacks *callbacks;
    GpuCallbacks *callbacks2;
    s32 value;
    s32 command;

    if (g_GraphDebug >= 2) {
        GPU_printf(D_800134AC, newValue);
    }

    callbacks = g_GpuFuncs;
    *state = newValue;
    value = callbacks->read(8);
    if (*state != 0) {
        command = value | 0x08000080;
    } else {
        command = value | 0x08000000;
    }
    g_GpuFuncs->submit(command);

    if (g_GraphType[0] == 2) {
        callbacks2 = g_GpuFuncs;
        if (g_GraphReverse != 0) {
            command = 0x20000501;
        } else {
            command = 0x20000504;
        }
        callbacks2->submit(command);
    }

    return old;
}

s32 SetGraphDebug(u8 arg0);

s32 SetGraphDebug(u8 arg0) {
    u8 *ptr = &g_GraphDebug;
    u8 old = *ptr;

    *ptr = arg0;
    if (arg0 != 0) {
        void (*func)(char *, ...) = GPU_printf;
        s32 a1;
        s32 a2;
        s32 a3;
        char *fmt;

        a1 = *ptr;
        a2 = g_GraphType[0];
        a3 = g_GraphReverse;
        fmt = D_800134C4;
        func(fmt, a1, a2, a3);
    }
    return old;
}
