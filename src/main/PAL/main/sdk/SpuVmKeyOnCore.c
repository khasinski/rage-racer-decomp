#include <sys/types.h>

#include "common.h"
#include "game/audio.h"

extern u_char g_SndVoiceRegs[] asm("D_8009DF20");
extern u_char g_SndVoiceStatePitch[] asm("D_8009E0BC");
extern volatile u_short D_8009E670;
extern volatile u_short D_8009E674;
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern u_char D_801E42F8;
extern volatile u_char *g_SndSpuRegs asm("D_8009A588");

void SpuVmKeyOnCore(long voice, u_short note, u_short fine, u_short left, u_short right) asm("func_80074818");
void SpuVmKeyOnCore(long voice, u_short note, u_short fine, u_short left, u_short right) {
    u_long rawVoice;
    /* This pin is load-bearing: removing it changes .text. */
    register u_long index asm("$2");
    u_long lowMask;
    u_long highMask;
    u_long i;
    /* These pins are load-bearing: removing any one changes .text. */
    register u_long count asm("$4");
    register u_long voiceIndex asm("$3");
    u_char stackPad[8];

    rawVoice = voice;
    voice = rawVoice & 0xFF;
    index = voice << 4;
    *(u_short *)&g_SndVoiceRegs[index + 2] = fine;
    g_SndVoiceFlags[voice] |= 3;
    *(u_short *)&g_SndVoiceRegs[index] = note;

    if ((u_long)voice < 16) {
        index = 1;
        lowMask = index << voice;
        highMask = 0;
    } else {
        lowMask = 0;
        voiceIndex = voice - 16;
        index = 1;
        highMask = index << voiceIndex;
    }

    voiceIndex = rawVoice & 0xFF;
    index = (((voiceIndex << 1) + voiceIndex) << 2) + voiceIndex;
    index <<= 2;
    count = D_801E42F8;
    i = 0;
    *(u_short *)&g_SndVoiceStatePitch[index] = 10;

    if (count != 0) {
        do {
            voiceIndex = (u_short)i;
            index = (((voiceIndex << 1) + voiceIndex) << 2) + voiceIndex;
            index <<= 2;
            g_SndVoiceStatePitch[index + 0x17] &= 1;
            i++;
            voiceIndex = D_801E42F8;
        } while ((u_short)i < voiceIndex);
    }

    voiceIndex = rawVoice & 0xFF;
    index = (((voiceIndex << 1) + voiceIndex) << 2) + voiceIndex;
    index <<= 2;
    g_SndVoiceStatePitch[index + 0x17] = 2;
    voiceIndex = D_8009E670;
    count = D_8009E674;
    *(u_short *)&g_SndVoiceStatePitch[index - 2] = 0;
    index = D_801F2A08;
    __asm__("or %0,%1,%2" : "=r"(voiceIndex) : "r"(lowMask), "r"(voiceIndex));
    __asm__("or %0,%1,%2" : "=r"(count) : "r"(highMask), "r"(count));
    D_8009E670 = voiceIndex;
    __asm__ volatile("" ::: "memory");
    index &= ~voiceIndex;
    D_8009E674 = count;
    __asm__ volatile("" ::: "memory");
    count = ~count;
    D_801F2A08 = index;
    index = D_801F2A0C;
    voiceIndex = (long)g_SndSpuRegs;
    index &= count;
    D_801F2A0C = index;
    *(u_short *)(voiceIndex + 0x194) = lowMask;
    *(u_short *)(voiceIndex + 0x196) = highMask;

    (void)stackPad;
}
