#include <sys/types.h>

#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

#define SND_KEY_ON_QUALIFIER volatile
#define SND_KEY_OFF_QUALIFIER volatile
#include "psyq/snd_internal.h"

void SpuVmKeyOnCore(long voice, u_short note, u_short fine, u_short left, u_short right) {
    u_long rawVoice;
    register u_long index asm("$2");
    u_long lowMask;
    u_long highMask;
    u_long i;
    register u_long count asm("$4");
    register u_long voiceIndex asm("$3");
    u_char stackPad[8];

    rawVoice = voice;
    voice = (u8)rawVoice;
    index = voice * 16;
    *(u_short *)((u_char *)g_SndVoiceRegs + index + 2) = fine;
    g_SndVoiceFlags[voice] |= 3;
    *(u_short *)((u_char *)g_SndVoiceRegs + index) = note;

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

    voiceIndex = (u8)rawVoice;
    index = ((voiceIndex * 3) << 2) + voiceIndex;
    index <<= 2;
    count = g_SndVoiceCount;
    i = 0;
    *(u_short *)&((u_char *)g_SndVoiceState + 4)[index] = 10;

    if (count != 0) {
        do {
            voiceIndex = (u_short)i;
            index = ((voiceIndex * 3) << 2) + voiceIndex;
            index <<= 2;
            ((u_char *)g_SndVoiceState + 4)[index + 0x17] &= 1;
            i++;
            voiceIndex = g_SndVoiceCount;
        } while ((u_short)i < voiceIndex);
    }

    voiceIndex = (u8)rawVoice;
    index = ((voiceIndex * 3) << 2) + voiceIndex;
    index <<= 2;
    ((u_char *)g_SndVoiceState + 4)[index + 0x17] = 2;
    voiceIndex = g_SndKeyOnLow;
    count = g_SndKeyOnHigh;
    *(u_short *)&((u_char *)g_SndVoiceState + 4)[index - 2] = 0;
    index = g_SndKeyOffLow;
    /* These barriers are load-bearing. Without them `combine` substitutes the
     * single-use `zero_extend(mem)` that defines the second operand into the
     * `ior`, which trips its "put the complex expression first" rule and swaps
     * the operands to `or rd,rt,rs`. Hiding the definition keeps the written
     * order, which is retail's `or rd,mask,bits`. */
    asm("" : "=r"(voiceIndex) : "0"(voiceIndex));
    voiceIndex = lowMask | voiceIndex;
    asm("" : "=r"(count) : "0"(count));
    count = highMask | count;
    g_SndKeyOnLow = voiceIndex;
    __asm__ volatile("" ::);
    index &= ~voiceIndex;
    g_SndKeyOnHigh = count;
    count = ~count;
    g_SndKeyOffLow = index;
    index = g_SndKeyOffHigh;
    voiceIndex = (long)g_SndSpuRegs;
    index &= count;
    g_SndKeyOffHigh = index;
    *(u_short *)(voiceIndex + 0x194) = lowMask;
    *(u_short *)(voiceIndex + 0x196) = highMask;

    (void)stackPad;
}
