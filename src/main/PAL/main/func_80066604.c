#include "common.h"

typedef struct Rect {
    s16 x;
    s16 y;
    u16 w;
    u16 h;
} Rect;

typedef struct DrawPacket {
    u8 pad0[3];
    u8 code;
    u32 x0y0;
    u32 x1y1;
} DrawPacket;

u32 Gpu_BuildDrawAreaTopLeftCmd(s32 x, s32 y) asm("func_80066A4C");
u32 Gpu_BuildDrawAreaBottomRightCmd(s32 x, s32 y) asm("func_80066B18");

void SetDrawArea(DrawPacket *pkt, Rect *rect) asm("func_80066604");

void SetDrawArea(DrawPacket *pkt, Rect *rect) {
    pkt->code = 2;
    pkt->x0y0 = Gpu_BuildDrawAreaTopLeftCmd(rect->x, rect->y);
    pkt->x1y1 = Gpu_BuildDrawAreaBottomRightCmd((s16)(rect->x + rect->w - 1), (s16)(rect->y + rect->h - 1));
}
