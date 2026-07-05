#include "common.h"

extern s32 D_801E4B34;

void func_80066604(void *packet, void *rect);
void func_80064DDC(void *ot, void *packet);

void *func_80034058(void *ot, void *packet, s16 x, s16 y, s32 w, s32 h) {
    register void *next asm("$16");
    register void *oldPacket asm("$5");
    s16 rect[4];
    s32 offset;

    offset = ((D_801E4B34 << 4) - D_801E4B34) << 4;
    rect[0] = x;
    rect[1] = y + offset;
    rect[2] = w;
    rect[3] = h;
    func_80066604(packet, rect);
    oldPacket = packet;
    next = packet;
    asm volatile("addiu %0,%0,12" : "=r"(next) : "0"(next));
    func_80064DDC(ot, oldPacket);
    return next;
}
