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
    u8 voiceByte = voice;
    s32 dfIndex = voiceByte << 3;
    s32 voiceOffset;
    s32 x;
    s32 sh4;
    s32 seg;
    u16 note;
    u16 pitch;
    s32 beaVal;
    u8 *pBd7;

    if (voiceByte >= 0x18U) {
        return;
    }

    voiceOffset = (((((voiceByte << 1) + voiceByte) << 2) + voiceByte) << 2);
    pBd7 = &D_801E4BD7;
    *pBd7 = D_8009E0C8[voiceOffset];
    D_801E4BDC = D_8009E0CC[voiceOffset];
    beaVal = voice & 0xFF;
    D_801E4BEA = beaVal;

    sh4 = *pBd7 << 4;
    seg = (s32)D_801E4BDC + sh4;
    x = (s16)arg1;
    if (x >= 0) {
        u8 *e = D_801E416C;
        s32 prodA = x * e[(seg << 5) + 0xD];
        s32 qA = prodA / 127;
        note = *(u16 *)((u8 *)D_8009E0C4 + voiceOffset) + qA;
        pitch = prodA - qA * 127;
    } else {
        u8 *e = D_801E416C;
        s32 prodB = x * e[(seg << 5) + 0xC];
        s32 qB = prodB / 127;
        note = *(u16 *)((u8 *)D_8009E0C4 + voiceOffset) + qB - 1;
        pitch = qB + 127;
    }

    *(volatile s16 *)(D_8009DF24 + (dfIndex << 1)) = func_80074A6C(note, pitch);
    D_8009E0A0[voice & 0xFF] |= 4;
}

void func_80074D0C(void) {
}

void func_80074D14(void) {
}
