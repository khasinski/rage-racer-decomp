#include "common.h"

typedef struct DebugCallbacks {
    s32 pad0[4];
    void (*submit)(s32);
    s32 pad14[5];
    s32 (*read)(s32);
} DebugCallbacks;

extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern DebugCallbacks *g_GpuFuncs asm("D_800941E0");
extern u8 g_GraphType asm("D_800941E8");
extern u8 g_GraphDebug asm("D_800941EA");
extern u8 g_GraphReverse asm("D_800941EB");
extern char D_800134AC[];

s32 SetGraphReverse(s32 arg0) asm("func_800655B8");

s32 SetGraphReverse(s32 arg0) {
    register s32 newValue asm("$17") = arg0;
    register u8 *state asm("$16") = &g_GraphReverse;
    register s32 old asm("$18") = *state;
    DebugCallbacks *callbacks;
    DebugCallbacks *callbacks2;
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

    if (g_GraphType == 2) {
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
