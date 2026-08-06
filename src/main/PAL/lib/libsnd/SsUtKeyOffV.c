#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

extern long g_SndUpdateLock;
extern u_short g_SndCurrentVoice;
extern u_short D_801F2A08;
extern u_short D_801F2A0C;
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_char g_SndVoiceState[];

long SsUtKeyOffV(long voice) {
    u_long value;
    u_long lowBits;
    u_long highBits;
    u_long channel;
    long offset;
    u_long activeLow;
    u_long activeHigh;
    u_long maskLow;
    u_long maskHigh;
    u_long keyLow;
    u_long keyHigh;

    if (g_SndUpdateLock != 1) {
        value = voice & 0xFFFF;
        g_SndUpdateLock = 1;
        if (value < 0x18) {
            g_SndCurrentVoice = voice;
            channel = g_SndCurrentVoice;
            if ((channel & 0xFFFF) < 0x10) {
                lowBits = 1 << (channel & 0xFFFF);
                /* lowBits and highBits are both live across the join, both
                 * referenced three times, and lowBits always dies first, so
                 * on live length alone lowBits outranks highBits and takes
                 * $6.  Retail has them the other way round.  The loop note
                 * is the lever: gcc 2.6.3 counts a reference once per loop
                 * depth, so wrapping this store takes highBits to four refs
                 * and inverts the two global-allocno priorities. */
                do {
                    highBits = 0;
                } while (0);
            } else {
                lowBits = 0;
                highBits = 1 << ((channel & 0xFFFF) - 0x10);
            }
            channel &= 0xFFFF;
            offset = channel * 52;
            g_SndVoiceStateStatus[offset] = 0;
            activeLow = D_801F2A08;
            activeHigh = D_801F2A0C;
            *(u_short *)&g_SndVoiceStatePitch[offset] = 0;
            *(u_short *)&g_SndVoiceState[offset] = 0;
            maskLow = D_8009E670;
            g_SndUpdateLock = 0;
            keyLow = lowBits | activeLow;
            D_801F2A08 = keyLow;
            D_8009E670 = maskLow & ~keyLow;
            maskHigh = D_8009E674;
            keyHigh = highBits | activeHigh;
            D_801F2A0C = keyHigh;
            D_8009E674 = maskHigh & ~keyHigh;
            return 0;
        }
        g_SndUpdateLock = 0;
    }
    return -1;
}
