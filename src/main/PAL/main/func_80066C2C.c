#include "common.h"

typedef unsigned int u_long;

typedef struct {
    u8 x;
    u8 pad1;
    u8 y;
    u8 pad3;
    s16 w;
    s16 h;
} RECT;

u_long Gpu_BuildTexWindowCmd(RECT *tw) asm("func_80066C2C");

u_long Gpu_BuildTexWindowCmd(RECT *tw) {
    u_long pad[4];

    if (tw != 0) {
        pad[0] = (tw->x & 0xFF) >> 3;
        pad[2] = (-tw->w & 0xFF) >> 3;
        pad[1] = (tw->y & 0xFF) >> 3;
        pad[3] = (-tw->h & 0xFF) >> 3;
        return 0xE2000000 | (pad[1] << 0xF) | (pad[0] << 0xA) |
               (pad[3] << 5) | pad[2];
    }

    return 0;
}
