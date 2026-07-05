#include "common.h"
#include "psyq/gpu.h"

u32 _get_mode(s32 arg0, s32 arg1, u32 arg2) asm("func_800669F0");
u32 Gpu_BuildDrawAreaTopLeftCmd(s32 x, s32 y) asm("func_80066A4C");
u32 Gpu_BuildDrawAreaBottomRightCmd(s32 x, s32 y) asm("func_80066B18");
u32 Gpu_BuildDrawOffsetCmd(s16 arg0, s16 arg1) asm("func_80066BE4");
u32 Gpu_BuildTexWindowCmd(void *arg0) asm("func_80066C2C");

void SetTexWindow(u8 *arg0, void *arg1) asm("func_800665C8");
void SetDrawArea(DrawPacket *pkt, Rect *rect) asm("func_80066604");
void SetDrawOffset(u8 *arg0, s16 *arg1) asm("func_80066688");
void SetDrawMask(u8 *arg0, s32 arg1, u32 arg2) asm("func_800666CC");
void Gpu_InitDrawModeTexWindowPacket(u8 *arg0, s32 arg1, s32 arg2, u16 arg3, void *arg4) asm("func_800666F4");

void SetTexWindow(u8 *arg0, void *arg1) {
    arg0[3] = 2;
    *(u32 *)(arg0 + 4) = Gpu_BuildTexWindowCmd(arg1);
    *(u32 *)(arg0 + 8) = 0;
}

void SetDrawArea(DrawPacket *pkt, Rect *rect) {
    pkt->code = 2;
    pkt->x0y0 = Gpu_BuildDrawAreaTopLeftCmd(rect->x, rect->y);
    pkt->x1y1 = Gpu_BuildDrawAreaBottomRightCmd((s16)(rect->x + rect->w - 1), (s16)(rect->y + rect->h - 1));
}

void SetDrawOffset(u8 *arg0, s16 *arg1) {
    arg0[3] = 2;
    *(u32 *)(arg0 + 4) = Gpu_BuildDrawOffsetCmd(arg1[0], arg1[1]);
    *(u32 *)(arg0 + 8) = 0;
}

void SetDrawMask(u8 *arg0, s32 arg1, u32 arg2) {
    u32 cmd;

    arg0[3] = 2;
    cmd = 0xE6000000;
    if (arg1 != 0) {
        cmd = 0xE6000002;
    }
    *(u32 *)(arg0 + 4) = cmd | (arg2 != 0);
    *(u32 *)(arg0 + 8) = 0;
}

void Gpu_InitDrawModeTexWindowPacket(u8 *arg0, s32 arg1, s32 arg2, u16 arg3, void *arg4) {
    arg0[3] = 2;
    *(u32 *)(arg0 + 4) = _get_mode(arg1, arg2, arg3);
    *(u32 *)(arg0 + 8) = Gpu_BuildTexWindowCmd(arg4);
}
