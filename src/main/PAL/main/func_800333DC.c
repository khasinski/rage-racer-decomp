#include "common.h"

extern u8 *D_8019C900;

void SetSprt(u8 *arg0) asm("func_80064FA8");
void SetShadeTex(u8 *arg0, s32 arg1) asm("func_80064EB8");
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_8001720C(void *ot, void *packet, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b);
void *func_80017390(void *ot, void *packet, s32 arg2);

void func_800333DC(void) {
    register u8 *packet __asm("$16");
    register u8 *next __asm("$17");
    register s32 i __asm("$18");
    register s32 x __asm("$19");
    register s32 u __asm("$20");
    register u8 *ot __asm("$4");
    register u8 *oldPacket __asm("$5");
    register s32 temp __asm("$2");
    register s32 uvOffset __asm("$3");
    u8 *ret;

    next = *(u8 **)0x1F800000;
    i = 0;
    u = 0x48;
    x = 0x6C;
    packet = next;

    do {
        SetSprt(next);
        SetShadeTex(next, 1);

        temp = 0x78;
        *(volatile s16 *)(packet + 0x0A) = temp;
        __asm__ volatile("" : : : "memory");
        uvOffset = (((i & 2) << 3) - (i & 2)) << 2;
        temp = -0x10 - uvOffset;
        uvOffset += 0x10;
        packet[0x0C] = temp;
        temp = 0x10;
        *(s16 *)(packet + 0x12) = temp;
        temp = 0x788C;
        oldPacket = packet;
        *(s16 *)(packet + 0x08) = x;
        packet[0x0D] = u;
        *(s16 *)(packet + 0x10) = uvOffset;
        *(s16 *)(packet + 0x0E) = temp;

        packet += 0x14;
        next += 0x14;
        u += 0x10;
        x += 0x10;
        ot = D_8019C900;
        i++;
        AddPrim(ot + 0xCC, oldPacket);
    } while (i < 3);

    ret = func_8001720C(D_8019C900 + 0xCC, next, 0x64, 0x70, 0x78, 0x20, 8, 8, 8);
    *(void **)0x1F800000 = ret;
    *(void **)0x1F800000 = func_80017390(D_8019C900 + 0xCC, ret, 9);
}
