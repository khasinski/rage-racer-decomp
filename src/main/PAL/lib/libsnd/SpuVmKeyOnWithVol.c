#include <sys/types.h>

#include "common.h"

extern u_char g_SndCurrentPriority;
extern u_char g_SndVoiceCount;
extern u_char g_SndCurrentVoice;
extern u_char g_SndVoiceStateStatus[];
extern u_char g_SndVoiceStatePitch[];
extern u_char *g_SndSpuRegs;

u_char SpuVmAlloc(long arg0);
void SpuVmKeyOnCore(long arg0, u_short arg1, u_short arg2, u_short arg3, u_short arg4);

void SpuVmKeyOnWithVol(long arg0, long arg1, long arg2, long arg3);
void SpuVmClearFinishedVoices(void);
void SpuVmKeyOnWithDefaultVol(long arg0, long arg1);

void SpuVmKeyOnWithVol(long arg0, long arg1, long arg2, long arg3) {
    long ret;

    g_SndCurrentPriority = 0x7F;
    ret = (u8)SpuVmAlloc(0xFF);
    *(u_short *)&g_SndCurrentVoice = ret;

    if (ret < g_SndVoiceCount) {
        SpuVmKeyOnCore(g_SndCurrentVoice, arg0, arg1, arg2, arg3);
    }
}

void SpuVmClearFinishedVoices(void) {
    long i;
    register long next asm("$2");
    long flag;
    long offset;
    long bound;
    u_char *ptr;

    i = 0;
    if (g_SndVoiceCount == 0) {
        return;
    }

    flag = 2;
    __asm__ volatile("" : "=r"(i) : "0"(i));
    next = i << 16;

for (;;) {
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
    bound = g_SndVoiceCount;
    next >>= 16;
    next = next < bound;
    if (next) {
        next = i << 16;
        continue;
    }
break;
}
}

void SpuVmKeyOnWithDefaultVol(long arg0, long arg1) {
    long ret;

    g_SndCurrentPriority = 0x7F;
    ret = (u8)SpuVmAlloc(0xFF);
    *(u_short *)&g_SndCurrentVoice = ret;

    if (ret < g_SndVoiceCount) {
        SpuVmKeyOnCore(g_SndCurrentVoice, arg0, arg1, 0x80FF, 0x5FC8);
    }
}
