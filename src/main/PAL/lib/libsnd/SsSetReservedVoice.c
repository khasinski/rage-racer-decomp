#include "psyq/spu.h"

extern u_char g_SndReservedVoiceCount asm("D_801E4D88");
extern short g_SndMonoMode asm("D_801E3FB0");
extern volatile u_char g_SndVoiceCount asm("D_801E42F8");
extern u_char g_SndVabStatus[] asm("D_801E4CFC");
extern u_short g_SndVabOpenCount asm("D_801F17AC");
extern u_long g_SndVabSpuAddr[] asm("D_801F17BC");

void SsSetReservedVoice(u_char arg0) asm("func_80072AD0");

void SsSetReservedVoice(u_char arg0) {
    g_SndReservedVoiceCount = arg0;
}

void SsSetMono(void) asm("func_80072AE0");

void SsSetMono(void) {
    g_SndMonoMode = 1;
}

void SsSetStereo(void) asm("func_80072AF4");

void SsSetStereo(void) {
    g_SndMonoMode = 0;
}

u_char SsSetVoiceCount(u_char voices) asm("func_80072B04");

u_char SsSetVoiceCount(u_char voices) {
    if (voices >= 25 || voices == 0) {
        return -1;
    }

    g_SndVoiceCount = voices;
    return g_SndVoiceCount;
}

void SsVabClose(short vab_id) asm("func_80072B3C");

void SsVabClose(short vab_id) {
    short id;

    if ((u_short)vab_id < 16) {
        id = vab_id;
        if (g_SndVabStatus[id] == 1) {
            SpuFree(g_SndVabSpuAddr[id]);
            g_SndVabStatus[id] = 0;
            g_SndVabOpenCount--;
        }
    }
}
