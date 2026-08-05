#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

extern SeqStruct *g_SndSeqTable[];
extern SpuVoice g_SndVoiceState[];
extern SvmCurrentAttr g_SndCurrentAttr;
extern u_short g_SndVoiceRegs[];
extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_char g_SndVoiceCount;
extern short g_SndMonoMode;
extern volatile u_short *g_SndSpuRegs;
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_short g_SndReverbOnLow;
extern u_short g_SndReverbOnHigh;

void SpuVmNoiseKeyOn(u_char voice);
void SpuVmNoiseKeyOn(u_char voice) {
    SeqStruct *score =
        &g_SndSeqTable[g_SndCurrentAttr.seq_sep & 0xFF]
                    [(g_SndCurrentAttr.seq_sep & 0xFF00) >> 8];
    short current_voice;
    u_short bits_upper;
    u_short bits_lower;
    u_long left_temp;
    u_long right_temp;
    u_long left;
    u_long right;
    u_short control;
    u_long pan;

    control = g_SndSpuRegs[0x1AA / 2];

    left_temp = score->left_volume * 0x81;
    right_temp = score->right_volume * 0x81;

    left_temp = (left_temp * g_SndCurrentAttr.master_volume) / 0x7F;
    right_temp = (right_temp * g_SndCurrentAttr.master_volume) / 0x7F;

    left_temp = (left_temp * g_SndCurrentAttr.tone_volume) / 0x7F;
    right_temp = (right_temp * g_SndCurrentAttr.tone_volume) / 0x7F;

    pan = g_SndCurrentAttr.tone_pan;
    if (pan < 0x40) {
        left = left_temp;
        right = (right_temp * pan) / 0x3F;
    } else {
        left = (left_temp * (0x7F - pan)) / 0x3F;
        right = right_temp;
    }

    pan = g_SndCurrentAttr.master_pan;
    if (pan < 0x40) {
        right = (right * pan) / 0x3F;
    } else {
        left = (left * (0x7F - pan)) / 0x3F;
    }

    pan = g_SndCurrentAttr.pan;
    if (pan < 0x40) {
        right = (pan * right) / 0x3F;
    } else {
        left = (left * (0x7F - pan)) / 0x3F;
    }

    if (g_SndMonoMode == 1) {
        if (left < right) {
            left = right;
        } else {
            right = left;
        }
    }

    control &= ~0x3F00;
    control |= ((g_SndCurrentAttr.note - g_SndCurrentAttr.center) & 0x3F) << 8;
    g_SndSpuRegs[0x1AA / 2] = control;

    g_SndVoiceRegs[voice * 8] = left;
    g_SndVoiceRegs[voice * 8 + 1] = right;
    g_SndVoiceFlags[voice] |= 3;

    if (voice < 0x10) {
        bits_lower = 1 << voice;
        bits_upper = 0;
    } else {
        bits_lower = 0;
        bits_upper = 1 << (voice - 0x10);
    }

    g_SndVoiceState[voice].pitch = 0xA;
    for (current_voice = 0; current_voice < g_SndVoiceCount; current_voice++) {
        g_SndVoiceState[current_voice].active &= 1;
    }
    g_SndVoiceState[voice].active = 2;

    D_8009E670 |= bits_lower;
    D_8009E674 |= bits_upper;
    D_801F2A08 &= ~D_8009E670;
    D_801F2A0C &= ~D_8009E674;

    if (g_SndCurrentAttr.mode & 4) {
        g_SndReverbOnLow |= bits_lower;
        g_SndReverbOnHigh |= bits_upper;
    } else {
        g_SndReverbOnLow &= ~bits_lower;
        g_SndReverbOnHigh &= ~bits_upper;
    }

    g_SndSpuRegs[0x194 / 2] = bits_lower;
    g_SndSpuRegs[0x196 / 2] = bits_upper;
}
