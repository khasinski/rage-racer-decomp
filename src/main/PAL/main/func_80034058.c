#include "common.h"

extern s32 D_801E4B34;

void func_80066604(void *packet, void *rect);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");

void *func_80034058(void *ot, void *packet, s16 x, s16 y, s32 w, s32 h) {
    void *oldPacket;
    s16 rect[4];
    s32 offset;

    offset = ((D_801E4B34 << 4) - D_801E4B34) << 4;
    rect[0] = x;
    rect[1] = y + offset;
    rect[2] = w;
    rect[3] = h;
    func_80066604(packet, rect);
    oldPacket = packet;
    packet = (u8 *)packet + 12;
    AddPrim(ot, oldPacket);
    return packet;
}
