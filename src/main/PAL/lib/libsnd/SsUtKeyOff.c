#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

#define SND_CURRENT_VOICE_QUALIFIER volatile
#include "psyq/snd_internal.h"

/* `voice` arrives as the voice index and is reused as its byte offset into
 * the 52-byte g_SndVoiceState* columns; voiceSlot keeps the plain index. */
long SsUtKeyOff(long voice, long vab_id, long program, long tone, long note) {
    short original_voice;
    long index;
    u_char voiceSlot;
    u_short bits_upper;
    u_short bits_lower;
    u_short current_voice;

    if (g_SndUpdateLock == 1) {
        return -1;
    }
    original_voice = voice;
    g_SndUpdateLock = 1;

    if ((u_short)original_voice < 24) {
        index = (short)voice;
        voice = (((index * 3) << 2) + index) << 2;
        if (*(short *)((u_char *)((u_char *)g_SndVoiceState + 22) + voice) == (short)vab_id &&
            *(short *)((u_char *)((u_char *)g_SndVoiceState + 18) + voice) == (short)program &&
            *(short *)((u_char *)((u_char *)g_SndVoiceState + 20) + voice) == (short)tone &&
            *(short *)((u_char *)((u_char *)g_SndVoiceState + 12) + voice) == (short)note) {
            if (*(short *)((u_char *)g_SndVoiceState + voice) == 0xFF) {
                voiceSlot = original_voice;
                g_SndVoiceState[voiceSlot].active = 0;
                g_SndVoiceState[voiceSlot].pitch = 0;
                ((u_short *)g_SndSpuRegs)[202] = 0;
                ((u_short *)g_SndSpuRegs)[203] = 0;
            } else {
                g_SndCurrentVoice = original_voice;
                current_voice = g_SndCurrentVoice;
                if (current_voice < 16) {
                    bits_lower = 1 << current_voice;
                    bits_upper = 0;
                } else {
                    bits_lower = 0;
                    bits_upper = 1 << (current_voice - 16);
                }
                g_SndVoiceState[current_voice].active = 0;
                g_SndVoiceState[current_voice].pitch = 0;
                g_SndVoiceState[current_voice].vag = 0;
                g_SndKeyOffLow = bits_lower | g_SndKeyOffLow;
                g_SndKeyOffHigh = bits_upper | g_SndKeyOffHigh;
                g_SndKeyOnLow &= ~g_SndKeyOffLow;
                g_SndKeyOnHigh &= ~g_SndKeyOffHigh;
            }
            g_SndUpdateLock = 0;
            return 0;
        }
    }
    g_SndUpdateLock = 0;
    return -1;
}
