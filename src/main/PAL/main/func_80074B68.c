#include "common.h"

extern u8 D_8009E0C8[];
extern s16 D_8009E0C4[];
extern u8 D_8009E0CC[];
extern volatile u8 D_8009DF24[];
extern volatile u8 D_8009E0A0[];
extern u8 D_801E4BD7;
extern u8 D_801E4BDC;
extern s16 D_801E4BEA;
extern u8 *D_801E416C;

u16 func_80074A6C(s32 arg0, s32 arg1);

void func_80074B68(s32 arg0, s32 arg1) {
    s32 voice = arg0;
    s32 dfIndex = (voice & 0xFF) << 3;
    s32 voiceOffset;
    register s32 x asm("$4");
    s32 sh4;
    register s32 seg asm("$6");
    register s32 base asm("$2");
    u16 note;
    u16 pitch;
    s32 beaVal;
    register u8 *pBd7 asm("$4");

    if ((u32)(voice & 0xFF) >= 0x18U) {
        return;
    }

    voiceOffset = (((((voice & 0xFF) << 1) + (voice & 0xFF)) << 2) + (voice & 0xFF)) << 2;
    pBd7 = &D_801E4BD7;
    *pBd7 = D_8009E0C8[voiceOffset];
    D_801E4BDC = D_8009E0CC[voiceOffset];
    __asm__("andi %0,%1,0xff" : "=r"(beaVal) : "r"(voice));
    D_801E4BEA = beaVal;

    sh4 = *pBd7 << 4;
    __asm__("addu %0,%1,%2" : "=r"(seg) : "r"((s32)D_801E4BDC), "r"(sh4));
    __asm__ volatile("sll $2,%1,16\n\tsra %0,$2,16" : "=r"(x) : "r"(arg1) : "$2");
    base = seg << 5;
    if (x >= 0) {
        register u8 *e asm("$3") = D_801E416C;
        s32 prodA = x * e[base + 0xD];
        s32 qA = prodA / 127;
        note = *(u16 *)((u8 *)D_8009E0C4 + voiceOffset) + qA;
        pitch = prodA - qA * 127;
    } else {
        register u8 *e asm("$3") = D_801E416C;
        s32 prodB = x * e[base + 0xC];
        s32 qB = prodB / 127;
        note = *(u16 *)((u8 *)D_8009E0C4 + voiceOffset) + qB - 1;
        pitch = qB + 127;
    }

    *(volatile s16 *)(D_8009DF24 + (dfIndex << 1)) = func_80074A6C(note, pitch);
    D_8009E0A0[voice & 0xFF] |= 4;
}
