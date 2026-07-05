#include "common.h"

extern u8 *D_8019C900;

void SetSemiTrans(u8 *arg0, s32 arg1) asm("func_80064E90");
void SetTile(u8 *arg0) asm("func_80064FF8");
void func_80064DDC(u32 *arg0, u32 *arg1);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void GameDrawFullscreenFadeTile(s32 color, s32 arg1) asm("func_80033AA0");
void GameDrawFullscreenFadeTile(s32 color, s32 arg1) {
    u8 *base = D_8019C900;
    u8 *ot = base + 0xCC;
    u8 *packet;
    u8 *next;
    register u8 *prim asm("$5");
    s32 height;

    if (color < 0) {
        color = 0;
    } else if (color >= 0x100) {
        color = 0xFF;
    }

    packet = *(u8 **)0x1F800000;
    SetTile(packet);
    SetSemiTrans(packet, 1);

    *(s16 *)(packet + 0xC) = 0x140;
    height = 0xF0;
    asm volatile("move %0,%1" : "=r"(prim) : "r"(packet));
    *(s16 *)(packet + 0x8) = 0;
    *(s16 *)(packet + 0xA) = 0;
    *(s16 *)(packet + 0xE) = height;
    packet[4] = color;
    packet[5] = color;
    packet[6] = color;

    asm volatile("" ::: "memory");
    asm volatile("addiu %0,%1,16" : "=r"(next) : "r"(packet));
    func_80064DDC((u32 *)ot, (u32 *)prim);
    *(void **)0x1F800000 = func_80017390(D_8019C900 + 0xCC, next, arg1);
}
