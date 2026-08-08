#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

#define SND_CURRENT_VOICE_TYPE u_short
#include "psyq/snd_internal.h"

long SsUtKeyOffV(long voice) {
    u_long value;
    u_long lowBits;
    u_long highBits;
    u_long channel;
    long offset;
    u_long activeLow;
    u_long activeHigh;

    if (g_SndUpdateLock != 1) {
        value = voice & 0xFFFF;
        g_SndUpdateLock = 1;
        if (value < 0x18) {
            g_SndCurrentVoice = voice;
            channel = g_SndCurrentVoice;
            if ((channel & 0xFFFF) < 0x10) {
                lowBits = 1 << (channel & 0xFFFF);
                highBits = 0;
            } else {
                lowBits = 0;
                highBits = 1 << ((channel & 0xFFFF) - 0x10);
            }
            channel &= 0xFFFF;
            offset = channel * 52;
            ((u_char *)g_SndVoiceState + 27)[offset] = 0;
            activeLow = g_SndKeyOffLow;
            activeHigh = g_SndKeyOffHigh;
            *(u_short *)&((u_char *)g_SndVoiceState + 4)[offset] = 0;
            *(u_short *)((u_char *)g_SndVoiceState + offset) = 0;
            /* Both key registers first, then both key-on masks: the same
             * four lines in the same order as SsUtKeyOff in sdk/, which is
             * the other place this block is written out.  Interleaved per
             * half instead, highBits' last reference lands nine insns past
             * lowBits', its live length goes 19 -> 23, and the two swap
             * registers. */
            g_SndKeyOffLow = lowBits | activeLow;
            g_SndKeyOffHigh = highBits | activeHigh;
            g_SndKeyOnLow &= ~g_SndKeyOffLow;
            g_SndKeyOnHigh &= ~g_SndKeyOffHigh;
            g_SndUpdateLock = 0;
            return 0;
        }
        g_SndUpdateLock = 0;
    }
    return -1;
}
