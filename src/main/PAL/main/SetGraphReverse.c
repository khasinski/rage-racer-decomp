#include "common.h"
#include "psyq/gpu.h"

extern void (* volatile GPU_printf)(char *, ...) asm("D_800941E4");
extern GpuCallbacks *g_GpuFuncs asm("D_800941E0");
extern u8 g_GraphType asm("D_800941E8");
extern u8 g_GraphDebug asm("D_800941EA");
extern u8 g_GraphReverse asm("D_800941EB");
extern char D_800134AC[];

s32 SetGraphReverse(s32 arg0) asm("func_800655B8");

s32 SetGraphReverse(s32 arg0) {
    register s32 newValue asm("$17") = arg0;
    register u8 *state asm("$16") = &g_GraphReverse;
    register s32 old asm("$18") = *state;
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

s32 SetGraphDebug(u8 arg0) asm("func_800656CC");

s32 SetGraphDebug(u8 arg0) {
    register volatile u8 *ptr asm("$3") = &g_GraphDebug;
    register s32 old asm("$16") = *ptr;

    *ptr = arg0;
    if (arg0 != 0) {
        void (*func)(char *, ...) = GPU_printf;
        register s32 a1 asm("$5");
        s32 a2;
        s32 a3;
        register char *fmt asm("$4");

        asm volatile("lbu $5,0($3)" : "=r"(a1) : "r"(ptr) : "memory");
        a2 = g_GraphType;
        a3 = g_GraphReverse;
        asm volatile(
            "lui $4,%%hi(D_800134C4)\n\t"
            "addiu $4,$4,%%lo(D_800134C4)"
            : "=r"(fmt)
            :
            : "memory");
        func(fmt, a1, a2, a3);
    }
    return old;
}
