#include "common.h"

extern u8 *D_8019C900;
extern s32 D_801E4D18;
extern s32 D_801E40D4;
extern u8 D_8007C728[];
extern u8 D_8007C738[];
extern u8 D_8007C739[];
extern u8 D_8007C73A[];

void SetTile(u8 *arg0) asm("func_80064FF8");
void AddPrim(u32 *arg0, u32 *arg1) asm("func_80064DDC");
s32 func_80016EC4(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);

void func_8001ACE4(u8 *packet) {
    register u8 *otArg asm("$4");
    register u8 *prim asm("$5");
    u8 *ot;
    register u8 *base asm("$2");
    u8 *base2;
    s32 colorIndex;
    register s32 paletteIndex asm("$3");
    s32 color;
    s32 next;

    base = D_8019C900;
    ot = base + 0xD0;

    SetTile(packet);
    otArg = ot;
    prim = packet;

    *(s16 *)(packet + 8) = 0x54;
    color = 0x98;
    packet[4] = 0;
    packet[5] = 0;
    packet[6] = 0;
    *(s16 *)(packet + 0xC) = color;
    *(s16 *)(packet + 0xA) = *(u16 *)&D_801E4D18 - 2;
    *(s16 *)(packet + 0xE) = 0x28;
    __asm__("" ::: "memory");
    packet += 0x10;
    AddPrim((u32 *)otArg, (u32 *)prim);

    colorIndex = D_8007C728[D_801E40D4];
    paletteIndex = (colorIndex * 2) + colorIndex;
    base2 = D_8019C900;
    ot = base2 + 0xBD0;
    next = func_80016EC4(ot, packet, 0x56, D_801E4D18, D_8007C73A[paletteIndex], 8, D_8007C738[paletteIndex], D_8007C739[paletteIndex], 0x7800);
    func_80017390(ot, next, 9);
}
