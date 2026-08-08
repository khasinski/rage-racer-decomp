#include "psyq/spu.h"
#include "psyq/snd.h"

#define SND_VOICE_COUNT_QUALIFIER volatile
#include "psyq/snd_internal.h"

void SsSetReservedVoice(u_char voices) {
    g_SndReservedVoiceCount = voices;
}

void SsSetMono(void) {
    g_SndMonoMode = 1;
}

void SsSetStereo(void) {
    g_SndMonoMode = 0;
}

u_char SsSetVoiceCount(u_char voices) {
    if (voices >= 25 || voices == 0) {
        return -1;
    }

    g_SndVoiceCount = voices;
    return g_SndVoiceCount;
}

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
