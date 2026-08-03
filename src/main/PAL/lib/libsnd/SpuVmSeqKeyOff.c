#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

extern u_char g_SndVoiceCount asm("D_801E42F8");
extern u_short g_SndCurrentVoice asm("D_801E4BEA");
extern u_short D_801F2A08;
extern u_short D_801F2A0C;
extern u_short D_8009E670;
extern u_short D_8009E674;
extern SpuVoice g_SndVoiceState[] asm("D_8009E0B8");

void SpuVmSeqKeyOff(long arg0) asm("func_80076ED8");

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

                D_801F2A08 = bits_lower | D_801F2A08;
                D_801F2A0C = bits_upper | D_801F2A0C;

                D_8009E670 &= ~D_801F2A08;
                D_8009E674 &= ~D_801F2A0C;
            }
            voice++;
        } while (voice < g_SndVoiceCount);
    }
}
