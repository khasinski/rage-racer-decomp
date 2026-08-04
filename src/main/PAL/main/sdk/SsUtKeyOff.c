#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

extern long g_SndUpdateLock asm("D_801E40AC");
extern u_short g_SndCurrentVoice asm("D_801E4BEA");
extern SpuVoice g_SndVoiceState[] asm("D_8009E0B8");
extern short g_SndVoiceStateNote[] asm("D_8009E0C4");
extern short g_SndVoiceStateProg[] asm("D_8009E0CA");
extern short g_SndVoiceStateTone[] asm("D_8009E0CC");
extern short g_SndVoiceStateVabId[] asm("D_8009E0CE");
extern u_short D_801F2A08;
extern u_short D_801F2A0C;
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_short *g_SndSpuRegs asm("D_8009A588");

long SsUtKeyOff(long voice, long vab_id, long program, long tone, long note);
long SsUtKeyOff(long voice, long vab_id, long program, long tone, long note) {
    short original_voice;
    long index;
    u_char new_var;
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
        if (*(short *)((u_char *)g_SndVoiceStateVabId + voice) == (short)vab_id &&
            *(short *)((u_char *)g_SndVoiceStateProg + voice) == (short)program &&
            *(short *)((u_char *)g_SndVoiceStateTone + voice) == (short)tone &&
            *(short *)((u_char *)g_SndVoiceStateNote + voice) == (short)note) {
            if (*(short *)((u_char *)g_SndVoiceState + voice) == 0xFF) {
                new_var = original_voice;
                g_SndVoiceState[new_var].active = 0;
                g_SndVoiceState[new_var].pitch = 0;
                g_SndSpuRegs[202] = 0;
                g_SndSpuRegs[203] = 0;
            } else {
                *(short *)&g_SndCurrentVoice = original_voice;
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
                D_801F2A08 = bits_lower | D_801F2A08;
                D_801F2A0C = bits_upper | D_801F2A0C;
                D_8009E670 &= ~D_801F2A08;
                D_8009E674 &= ~D_801F2A0C;
            }
            g_SndUpdateLock = 0;
            return 0;
        }
    }
    g_SndUpdateLock = 0;
    return -1;
}
