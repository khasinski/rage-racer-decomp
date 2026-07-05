#include "common.h"

extern char D_80013504[];
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern u32 g_DrawSyncCallback asm("D_800941F4");
extern u8 g_GraphDebug asm("D_800941EA");

u32 DrawSyncCallback(u32 arg0) asm("func_80065804");
u32 DrawSyncCallback(u32 arg0) {
    u32 ret;

    if (g_GraphDebug >= 2) {
        GPU_printf(D_80013504, arg0);
    }

    ret = g_DrawSyncCallback;
    g_DrawSyncCallback = arg0;
    return ret;
}
