#include <sys/types.h>

#include "common.h"

extern volatile long g_SndUpdateLock asm("D_801E40AC");
extern volatile u_short g_SndCurrentVoice asm("D_801E4BEA");
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern volatile u_short D_8009E670;
extern volatile u_short D_8009E674;
extern u_char g_SndVoiceStateStatus[] asm("D_8009E0D3");
extern u_char g_SndVoiceStatePitch[] asm("D_8009E0BC");
extern u_char g_SndVoiceState[] asm("D_8009E0B8");

long SsUtKeyOffV(long voice) asm("func_80078018");

long SsUtKeyOffV(long arg0) {
    /* These pins are load-bearing: removing any one changes .text. */
    register long one asm("$5") = 1;
    u_long value;
    register u_long highBits asm("$6");
    u_long lowBits;
    register long offset asm("$3");
    register u_long activeLow asm("$4");
    register u_long activeHigh asm("$5");
    register u_long maskLow asm("$3");
    register u_long maskHigh asm("$3");

    if (g_SndUpdateLock != one) {
        value = arg0 & 0xFFFF;
        asm volatile("" : "=r"(value) : "0"(value));
        g_SndUpdateLock = one;
        if (value < 0x18) {
            /* This pin is load-bearing: removing it changes .text. */
            register u_long channel asm("$4");
            u_long masked;

            g_SndCurrentVoice = arg0;
            channel = g_SndCurrentVoice;
            asm volatile("" : "=r"(channel) : "0"(channel));
            masked = channel & 0xFFFF;
            asm volatile("" : "=r"(masked) : "0"(masked));

            if (masked < 0x10) {
                lowBits = one << masked;
                highBits = 0;
            } else {
                /* This pin is load-bearing: removing it changes .text. */
                register u_long temp asm("$2");

                lowBits = 0;
                temp = masked - 0x10;
                highBits = one << temp;
            }

            channel &= 0xFFFF;
            offset = ((((channel * 3) * 4) + channel) * 4);
            g_SndVoiceStateStatus[offset] = 0;
            activeLow = D_801F2A08;
            activeHigh = D_801F2A0C;
            *(u_short *)&g_SndVoiceStatePitch[offset] = 0;
            *(u_short *)&g_SndVoiceState[offset] = 0;

            maskLow = D_8009E670;
            g_SndUpdateLock = 0;
            asm volatile("" ::: "memory");
            activeLow = lowBits | activeLow;
            D_801F2A08 = activeLow;
            D_8009E670 = maskLow & ~activeLow;

            maskHigh = D_8009E674;
            asm volatile("" : "=r"(maskHigh) : "0"(maskHigh));
            activeHigh = highBits | activeHigh;
            D_801F2A0C = activeHigh;
            D_8009E674 = maskHigh & ~activeHigh;
            asm volatile("" ::: "memory");
            return 0;
        }
        g_SndUpdateLock = 0;
    }
    return -1;
}
