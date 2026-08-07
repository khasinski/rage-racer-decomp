#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"
#include "psyq/snd.h"

extern u_char g_SndVoiceCount;
extern u_short g_SndCurrentVoice;
extern u_short g_SndKeyOffLow;
extern u_short g_SndKeyOffHigh;
extern u_short g_SndKeyOnLow;
extern u_short g_SndKeyOnHigh;
extern SpuVoice g_SndVoiceState[];

void SpuVmSeqKeyOff(long arg0) {
    u_short bits_upper;
    u_short bits_lower;
    u_char voice;
    u_short selected_voice;
    u_long selected_index;
    long seq_sep;
    u_short *current;

    voice = 0;
    if (g_SndVoiceCount != 0) {
        seq_sep = (short)arg0;
        current = &g_SndCurrentVoice;
        do {
            if (g_SndVoiceState[voice].seq_sep == seq_sep) {
                *current = voice;
                selected_voice = g_SndCurrentVoice;
                selected_index = selected_voice;
                if (selected_index < 0x10) {
                    bits_lower = 1 << selected_index;
                    bits_upper = 0;
                } else {
                    bits_lower = 0;
                    bits_upper = 1 << (selected_index - 0x10);
                }

                g_SndVoiceState[selected_voice].active = 0;
                g_SndVoiceState[selected_voice].pitch = 0;
                g_SndVoiceState[selected_voice].vag = 0;

                g_SndKeyOffLow = bits_lower | g_SndKeyOffLow;
                g_SndKeyOffHigh = bits_upper | g_SndKeyOffHigh;

                g_SndKeyOnLow &= ~g_SndKeyOffLow;
                g_SndKeyOnHigh &= ~g_SndKeyOffHigh;
            }
            voice++;
        } while (voice < g_SndVoiceCount);
    }
}
