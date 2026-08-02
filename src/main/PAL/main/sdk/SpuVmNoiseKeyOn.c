#include <sys/types.h>

#include "common.h"

typedef struct SeqState74348 {
    u_char pad00[0x74];
    u_short left_volume;
    u_short right_volume;
    u_char pad78[0x34];
} SeqState74348;

typedef struct VoiceState74348 {
    short vag;
    short age;
    short pitch;
    u_short unk6;
    short base_volume;
    signed char unkA;
    signed char unkB;
    short note;
    short seq_sep;
    short program_index;
    short program;
    short tone;
    short vab_id;
    short priority;
    u_char pad1A;
    u_char status;
    short auto_volume;
    short unk1E;
    short unk20;
    short unk22;
    short start_volume;
    short end_volume;
    short auto_pan;
    short unk2A;
    short unk2C;
    short unk2E;
    short start_pan;
    short end_pan;
} VoiceState74348;

typedef struct SvmCurrent74348 {
    u_char tones;
    u_char vab_id;
    u_char note;
    u_char fine;
    u_char volume;
    u_char pan;
    u_char program;
    u_char fake_program;
    u_char unk8;
    u_char unk9;
    u_char master_volume;
    u_char master_pan;
    u_char tone;
    u_char tone_volume;
    u_char tone_pan;
    u_char priority;
    u_char center;
    u_char shift;
    u_char min;
    u_char max;
    u_char mode;
    u_char pad15;
    u_short seq_sep;
    short vag;
    u_short voice;
} SvmCurrent74348;

extern SeqState74348 *g_SndSeqTable[] asm("D_801E79CC");
extern VoiceState74348 g_SndVoiceState[] asm("D_8009E0B8");
extern SvmCurrent74348 g_SndCurrentAttr asm("D_801E4BD0");
extern u_short g_SndVoiceRegs[] asm("D_8009DF20");
extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_char D_801E42F8;
extern short g_SndMonoMode asm("D_801E3FB0");
extern volatile u_short *g_SndSpuRegs asm("D_8009A588");
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_short g_SndReverbOnLow asm("D_8009E680");
extern u_short g_SndReverbOnHigh asm("D_8009E684");

void SpuVmNoiseKeyOn(u_char voice) asm("func_80074348");
void SpuVmNoiseKeyOn(u_char voice) {
    SeqState74348 *score =
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
    for (current_voice = 0; current_voice < D_801E42F8; current_voice++) {
        g_SndVoiceState[current_voice].status &= 1;
    }
    g_SndVoiceState[voice].status = 2;

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
