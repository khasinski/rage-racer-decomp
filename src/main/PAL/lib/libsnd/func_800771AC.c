#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

extern SeqStruct *g_SndSeqTable[];
extern SpuVoice g_SndVoiceState[];
extern short g_SndVoiceRegs[];
extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_char g_SndVoiceCount;
extern short g_SndMonoMode;
extern short g_SndCurrentSeqSep;
extern ProgAtr *g_SndCurrentProgTable;
extern VabHdr *g_SndCurrentVabHeader;
extern VagAtr *g_SndCurrentToneTable;
extern long g_SndUpdateLock;
extern SvmCurrentAttr g_SndCurrentAttr;

long SpuVmVSetUp(short vab_id, short program);
u_char SpuVmAlloc(long priority);
void SpuVmRebuildVoiceTable(void);
void SpuVmNoiseKeyOn(long voice);
long SpuVmCalculateTonePitch(u_short note, u_short fine);
void SpuVmScaleVabVolume(long count, long pitch);

long SsSeqIndexChannel(short seq_sep, short vab_id, short program, u_short volume, u_short pan) {
    SeqStruct *score =
        &g_SndSeqTable[seq_sep & 0xFF][(seq_sep & 0xFF00) >> 8];
    long voices_updated;
    u_char voice;
    long base_volume;
    u_long left;
    u_long right;
    u_long left_scaled;
    u_long right_scaled;
    long master_volume;
    u_char current_pan;
    short current_program;

    voices_updated = 0;
    SpuVmVSetUp(vab_id, program);
    g_SndCurrentSeqSep = seq_sep;
    for (voice = 0; voice < g_SndVoiceCount; voice++) {
        current_program = program;
        if ((g_SndVoiceState[voice].seq_sep == seq_sep) &&
            (g_SndVoiceState[voice].program == current_program) &&
            (g_SndVoiceState[voice].vab_id == vab_id)) {
            if (score->vol[score->channel] != volume &&
                score->vol[score->channel] == 0) {
                score->vol[score->channel] = 1;
            }
            base_volume = (g_SndVoiceState[voice].base_volume * volume) / 0x7F;
            master_volume = g_SndCurrentVabHeader->mvol * 0x3FFF;
            left_scaled = ((base_volume * master_volume) / 0x7F) / 0x7F;
            left_scaled =
                ((left_scaled * g_SndCurrentProgTable[current_program].mvol *
                  g_SndCurrentToneTable[g_SndVoiceState[voice].tone +
                              (current_program * 0x10)]
                      .vol) /
                 0x7F) /
                0x7F;
            right_scaled = left_scaled;

            left_scaled = (left_scaled * score->left_volume) / 0x7F;
            right_scaled = (right_scaled * score->right_volume) / 0x7F;

            current_pan = g_SndCurrentToneTable[g_SndVoiceState[voice].tone].pan;
            if (current_pan < 0x40) {
                left = left_scaled;
                right = (right_scaled * current_pan) / 0x3F;
            } else {
                left = (left_scaled * (0x7F - current_pan)) / 0x3F;
                right = right_scaled;
            }
            current_pan = g_SndCurrentProgTable[g_SndVoiceState[voice].program_index].mpan;
            if (current_pan < 0x40) {
                right = (right * current_pan) / 0x3F;
            } else {
                left = (left * (0x7F - current_pan)) / 0x3F;
            }
            do {
                do {
                    current_pan = pan;
                } while (0);
            } while (0);
            if (current_pan < 0x40) {
                right = (right * current_pan) / 0x3F;
            } else {
                left = (left * (0x7F - current_pan)) / 0x3F;
            }
            if (g_SndMonoMode == 1) {
                if (left < right) {
                    left = right;
                } else {
                    right = left;
                }
            }
            left = (left * left) / 0x3FFF;
            right = (right * right) / 0x3FFF;
            g_SndVoiceRegs[voice * 8] = left;
            g_SndVoiceRegs[voice * 8 + 1] = right;
            g_SndVoiceFlags[voice] |= 3;
            voices_updated++;
        }
    }
    return voices_updated;
}

short SsUtKeyOn(short, short, short, short, short, short, short);
short SsUtKeyOn(
    short vab_id,
    short program,
    short tone,
    short note,
    short fine,
    short left,
    short right) {
    u_short voice;
    short voice_index;
    short vag;
    long tone_index;
    long left_value;
    long right_value;

    if (g_SndUpdateLock == 1) {
        return -1;
    }
    g_SndUpdateLock = 1;

    if (SpuVmVSetUp(vab_id, program)) {
        g_SndUpdateLock = 0;
        return -1;
    }
    g_SndCurrentAttr.seq_sep = 0x21;
    g_SndCurrentAttr.note = note;
    g_SndCurrentAttr.fine = fine;
    g_SndCurrentAttr.tone = tone;

    left_value = left;
    right_value = right;
    if (left_value == right_value) {
        g_SndCurrentAttr.pan = 0x40;
        g_SndCurrentAttr.volume = left;
    } else if (right_value < left_value) {
        g_SndCurrentAttr.volume = left;
        g_SndCurrentAttr.pan = (right_value * 0x40) / left_value;
    } else {
        g_SndCurrentAttr.volume = right;
        g_SndCurrentAttr.pan = 0x7F - ((left_value * 0x40) / right_value);
    }

    g_SndCurrentAttr.master_volume = g_SndCurrentProgTable[program].mvol;
    g_SndCurrentAttr.master_pan = g_SndCurrentProgTable[program].mpan;
    g_SndCurrentAttr.tones = g_SndCurrentProgTable[program].tones;

    tone_index = g_SndCurrentAttr.tone + (g_SndCurrentAttr.program_index * 0x10);
    g_SndCurrentAttr.priority = g_SndCurrentToneTable[tone_index].prior;
    vag = g_SndCurrentToneTable[tone_index].vag;
    g_SndCurrentAttr.vag = vag;
    g_SndCurrentAttr.tone_volume = g_SndCurrentToneTable[tone_index].vol;
    g_SndCurrentAttr.tone_pan = g_SndCurrentToneTable[tone_index].pan;
    g_SndCurrentAttr.center = g_SndCurrentToneTable[tone_index].center;
    g_SndCurrentAttr.shift = g_SndCurrentToneTable[tone_index].shift;
    g_SndCurrentAttr.mode = g_SndCurrentToneTable[tone_index].mode;
    g_SndCurrentAttr.min = g_SndCurrentToneTable[tone_index].min;
    g_SndCurrentAttr.max = g_SndCurrentToneTable[tone_index].max;

    if (vag == 0) {
        g_SndUpdateLock = 0;
        return -1;
    }

    voice = SpuVmAlloc(vag);
    voice_index = voice;
    if (voice_index == g_SndVoiceCount) {
        g_SndUpdateLock = 0;
        return -1;
    }

    g_SndCurrentAttr.voice = voice;
    g_SndVoiceState[voice_index].seq_sep = 0x21;
    g_SndVoiceState[voice_index].vab_id = vab_id;
    g_SndVoiceState[voice_index].program_index = g_SndCurrentAttr.program_index;
    g_SndVoiceState[voice_index].program = program;
    g_SndVoiceState[voice_index].vag = g_SndCurrentAttr.vag;
    g_SndVoiceState[voice_index].tone = g_SndCurrentAttr.tone;
    g_SndVoiceState[voice_index].note = note;
    g_SndVoiceState[voice_index].active = 1;
    g_SndVoiceState[voice_index].age = 0;

    SpuVmRebuildVoiceTable();
    if ((short)g_SndCurrentAttr.vag == 0xFF) {
        SpuVmNoiseKeyOn(voice);
    } else {
        SpuVmScaleVabVolume(1, SpuVmCalculateTonePitch(note, fine) & 0xFFFF);
    }
    g_SndUpdateLock = 0;
    return voice;
}
