#include <sys/types.h>

#include "common.h"

extern u_char D_8009E0C8[];
extern short D_8009E0C4[];
extern u_char D_8009E0CC[];
extern volatile u_char D_8009DF24[];
extern volatile u_char D_8009E0A0[];
extern u_char D_801E4BD7;
extern u_char D_801E4BDC;
extern short D_801E4BEA;
extern u_char *D_801E416C;

u_short func_80074A6C(long arg0, long arg1);

void SpuVmPitchBendVoice(long arg0, long arg1) asm("func_80074B68");
void SpuVmPitchBendVoice(long arg0, long arg1) {
    long voice = arg0;
    u_char voiceByte = voice;
    long dfIndex = voiceByte << 3;
    long voiceOffset;
    long x;
    long sh4;
    long seg;
    u_short note;
    u_short pitch;
    long beaVal;
    u_char *pBd7;

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
    seg = (long)D_801E4BDC + sh4;
    x = (short)arg1;
    if (x >= 0) {
        u_char *e = D_801E416C;
        long prodA = x * e[(seg << 5) + 0xD];
        long qA = prodA / 127;
        note = *(u_short *)((u_char *)D_8009E0C4 + voiceOffset) + qA;
        pitch = prodA - qA * 127;
    } else {
        u_char *e = D_801E416C;
        long prodB = x * e[(seg << 5) + 0xC];
        long qB = prodB / 127;
        note = *(u_short *)((u_char *)D_8009E0C4 + voiceOffset) + qB - 1;
        pitch = qB + 127;
    }

    *(volatile short *)(D_8009DF24 + (dfIndex << 1)) = func_80074A6C(note, pitch);
    D_8009E0A0[voice & 0xFF] |= 4;
}

void func_80074D0C(void) {
}

void func_80074D14(void) {
}
