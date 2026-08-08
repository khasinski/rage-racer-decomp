#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

#define SND_CURRENT_VOICE_TYPE u_char
#include "psyq/snd_internal.h"

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
        if (((u_char *)g_SndVoiceState + 27)[offset] == 2) {
            offset = ((u_char)i) * 0x34;
            ((u_char *)g_SndVoiceState + 27)[offset] = 0;
            ptr = g_SndSpuRegs;
            *(u_short *)(((u_char *)g_SndVoiceState + 4) + offset) = 0;
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
