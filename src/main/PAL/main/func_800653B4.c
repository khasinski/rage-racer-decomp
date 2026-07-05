#include "common.h"

extern char D_800133F4[];
extern char D_80013410[];
extern char D_8001342C[];
extern char D_80013438[];
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");

typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
    u8 b0;
    u8 b1;
} UnkFunc800653B4;

void DumpDispEnv(UnkFunc800653B4 *arg0) asm("func_800653B4");

void DumpDispEnv(UnkFunc800653B4 *arg0) {
    GPU_printf(D_800133F4, arg0->x0, arg0->y0, arg0->x1, arg0->y1);
    GPU_printf(D_80013410, arg0->x2, arg0->y2, arg0->x3, arg0->y3);
    GPU_printf(D_8001342C, arg0->b0);
    GPU_printf(D_80013438, arg0->b1);
}
