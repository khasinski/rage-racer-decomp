#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

extern SpuVoice g_SndVoiceState[];
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_short *g_SndSpuRegs;
extern u_char g_SndVoiceCount;
extern SvmCurrentAttr g_SndCurrentAttr;
extern u_short D_801F2A08;
extern u_short D_801F2A0C;

static inline u_short get_selected_voice(void) {
    return g_SndCurrentAttr.voice;
}

long SpuVmSeKeyOff(short seq_sep, short vab_id, short program, u_short note);
long SpuVmSeKeyOff(short seq_sep, short vab_id, short program, u_short note) {
    u_short bits_upper;
    u_short bits_lower;
    u_char voice;
    long count;
    u_short selected_voice;
    u_long selected_index;

    count = 0;
    for (voice = 0; voice < g_SndVoiceCount; voice++) {
        if ((g_SndVoiceState[voice].note == note) &&
            (g_SndVoiceState[voice].program == program) &&
            (g_SndVoiceState[voice].seq_sep == seq_sep) &&
            (g_SndVoiceState[voice].vab_id == vab_id)) {
            if (g_SndVoiceState[voice].vag == 0xFF) {
                g_SndVoiceState[voice].active = 0;
                g_SndVoiceState[voice].pitch = 0;
                g_SndSpuRegs[202] = 0;
                g_SndSpuRegs[203] = 0;
            } else {
                g_SndCurrentAttr.voice = voice;
                selected_voice = get_selected_voice();
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
            count++;
        }
    }
    return count;
}
