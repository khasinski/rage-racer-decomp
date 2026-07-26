#include "common.h"
#include "psyq/gpu.h"

u32 func_800669F0(s32 arg0, s32 arg1, u32 arg2);
u32 func_80066A4C(s32 x, s32 y);
u32 func_80066B18(s32 x, s32 y);
u32 func_80066BE4(s16 arg0, s16 arg1);
u32 func_80066C2C(void *arg0);

void SetTexWindow(DrawPacket *pkt, void *arg1) asm("func_800665C8");
void SetTexWindow(DrawPacket *pkt, void *arg1) {
    pkt->code = 2;
    pkt->x0y0 = func_80066C2C(arg1);
    pkt->x1y1 = 0;
}

void SetDrawArea(DrawPacket *pkt, Rect *rect) asm("func_80066604");
void SetDrawArea(DrawPacket *pkt, Rect *rect) {
    pkt->code = 2;
    pkt->x0y0 = func_80066A4C(rect->x, rect->y);
    pkt->x1y1 = func_80066B18((s16)(rect->x + rect->w - 1), (s16)(rect->y + rect->h - 1));
}

void SetDrawOffset(DrawPacket *pkt, s16 *arg1) asm("func_80066688");
void SetDrawOffset(DrawPacket *pkt, s16 *arg1) {
    pkt->code = 2;
    pkt->x0y0 = func_80066BE4(arg1[0], arg1[1]);
    pkt->x1y1 = 0;
}

void func_800666CC(DrawPacket *pkt, s32 arg1, u32 arg2) {
    u32 cmd;

    pkt->code = 2;
    cmd = 0xE6000000;
    if (arg1 != 0) {
        cmd = 0xE6000002;
    }
    pkt->x0y0 = cmd | (arg2 != 0);
    pkt->x1y1 = 0;
}

void SetDrawMode(DrawPacket *pkt, s32 arg1, s32 arg2, u16 arg3, void *arg4) asm("func_800666F4");
void SetDrawMode(DrawPacket *pkt, s32 arg1, s32 arg2, u16 arg3, void *arg4) {
    pkt->code = 2;
    pkt->x0y0 = func_800669F0(arg1, arg2, arg3);
    pkt->x1y1 = func_80066C2C(arg4);
}
