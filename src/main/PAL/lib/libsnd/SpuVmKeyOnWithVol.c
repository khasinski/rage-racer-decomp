#include <sys/types.h>

#include "common.h"

extern u_char g_SndCurrentPriority asm("D_801E4BDF");
extern u_char D_801E42F8;
extern u_char g_SndCurrentVoice asm("D_801E4BEA");
extern u_char g_SndVoiceStateStatus[] asm("D_8009E0D3");
extern u_char g_SndVoiceStatePitch[] asm("D_8009E0BC");
extern u_char *g_SndSpuRegs asm("D_8009A588");

u_char SpuVmAlloc(long arg0) asm("func_800739E8");
void SpuVmKeyOnCore(long arg0, u_short arg1, u_short arg2, u_short arg3, u_short arg4) asm("func_80074818");

void SpuVmKeyOnWithVol(long arg0, long arg1, long arg2, long arg3) asm("func_80075AE0");
void SpuVmClearFinishedVoices(void) asm("func_80075B78");
void SpuVmKeyOnWithDefaultVol(long arg0, long arg1) asm("func_80075C30");

void SpuVmKeyOnWithVol(long arg0, long arg1, long arg2, long arg3) {
    long ret;

    g_SndCurrentPriority = 0x7F;
    ret = SpuVmAlloc(0xFF) & 0xFF;
    *(u_short *)&g_SndCurrentVoice = ret;

    if (ret < D_801E42F8) {
        SpuVmKeyOnCore(g_SndCurrentVoice, arg0, arg1, arg2, arg3);
    }
}

void SpuVmClearFinishedVoices(void) {
    long i;
    /* This pin is load-bearing: removing it changes .text. */
    register long next asm("$2");
    long flag;
    long offset;
    long bound;
    u_char *ptr;

    i = 0;
    if (D_801E42F8 == 0) {
        return;
    }

    flag = 2;
    __asm__ volatile("" : "=r"(i) : "0"(i));
    next = i << 16;

loop:
    next >>= 16;
    offset = next * 0x34;
    if (g_SndVoiceStateStatus[offset] == flag) {
        offset = ((u_char)i) * 0x34;
        g_SndVoiceStateStatus[offset] = 0;
        ptr = g_SndSpuRegs;
        *(u_short *)(g_SndVoiceStatePitch + offset) = 0;
        *(u_short *)(ptr + 0x194) = 0;
        *(u_short *)(ptr + 0x196) = 0;
    }

    next = i + 1;
    i = next;
    next <<= 16;
    bound = D_801E42F8;
    next >>= 16;
    next = next < bound;
    if (next) {
        next = i << 16;
        goto loop;
    }
}

void SpuVmKeyOnWithDefaultVol(long arg0, long arg1) {
    long ret;

    g_SndCurrentPriority = 0x7F;
    ret = SpuVmAlloc(0xFF) & 0xFF;
    *(u_short *)&g_SndCurrentVoice = ret;

    if (ret < D_801E42F8) {
        SpuVmKeyOnCore(g_SndCurrentVoice, arg0, arg1, 0x80FF, 0x5FC8);
    }
}
