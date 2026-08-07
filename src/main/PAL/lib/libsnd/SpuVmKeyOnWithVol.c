#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

extern u_char g_SndVoiceCount;
extern u_char g_SndCurrentVoice;
extern u_char *g_SndSpuRegs;

void SpuVmKeyOnWithVol(long note, long fine, long left, long right) {
    long ret;

    g_SndCurrentPriority = 0x7F;
    ret = (u8)SpuVmAlloc(0xFF);
    *(u_short *)&g_SndCurrentVoice = ret;

    if (ret < g_SndVoiceCount) {
        SpuVmKeyOnCore(g_SndCurrentVoice, note, fine, left, right);
    }
}

void SpuVmClearFinishedVoices(void) {
    short i;
    long offset;
    u_char *ptr;

    for (i = 0; i < g_SndVoiceCount; i++) {
        offset = i * 0x34;
        if (g_SndVoiceStateStatus[offset] == 2) {
            offset = ((u_char)i) * 0x34;
            g_SndVoiceStateStatus[offset] = 0;
            ptr = g_SndSpuRegs;
            *(u_short *)(g_SndVoiceStatePitch + offset) = 0;
            *(u_short *)(ptr + 0x194) = 0;
            *(u_short *)(ptr + 0x196) = 0;
        }
    }
}

void SpuVmKeyOnWithDefaultVol(long note, long fine) {
    long ret;

    g_SndCurrentPriority = 0x7F;
    ret = (u8)SpuVmAlloc(0xFF);
    *(u_short *)&g_SndCurrentVoice = ret;

    if (ret < g_SndVoiceCount) {
        SpuVmKeyOnCore(g_SndCurrentVoice, note, fine, 0x80FF, 0x5FC8);
    }
}
