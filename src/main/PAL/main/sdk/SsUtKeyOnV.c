#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"


/* The key-on path's view of the same block psyq/snd_types.h calls
 * SvmCurrentAttr, but four bytes longer and reading vag unsigned, so the two
 * are not interchangeable. One of them is wrong. */
extern long g_SndUpdateLock;
extern ProgAtr *g_SndCurrentProgTable;
extern VagAtr *g_SndCurrentToneTable;
extern SvmCurrentAttr g_SndCurrentAttr;
extern SpuVoice g_SndVoiceState[];

long SsUtKeyOnV(
    long voice,
    short vab_id,
    short program,
    short tone,
    u_short note,
    u_short fine,
    short left,
    short right) {
    long idx;
    u_char tone_value;
    u_short vag;
    ProgAtr *program_attr;
    VagAtr *tone_attr;
    long left_value;
    long right_value;

    if (g_SndUpdateLock == 1) {
        return -1;
    }
    g_SndUpdateLock = 1;
    if ((u_short)voice >= 24) {
        g_SndUpdateLock = 0;
        return -1;
    }
    if (SpuVmVSetUp(vab_id, program) != 0) {
        g_SndUpdateLock = 0;
        return -1;
    }
    g_SndCurrentAttr.seq_sep = 0x21;
    left_value = left;
    right_value = right;
    g_SndCurrentAttr.note = note;
    g_SndCurrentAttr.fine = fine;
    g_SndCurrentAttr.tone = tone;

    if (left_value == right_value) {
        g_SndCurrentAttr.pan = 0x40;
        g_SndCurrentAttr.volume = left;
    } else if (right_value < left_value) {
        g_SndCurrentAttr.volume = left;
        g_SndCurrentAttr.pan = (right_value * 0x40) / left_value;
    } else {
        g_SndCurrentAttr.volume = right;
        g_SndCurrentAttr.pan =
            0x7F - ((left_value * 0x40) / right_value);
    }

    program_attr = &g_SndCurrentProgTable[program];
    g_SndCurrentAttr.master_volume = program_attr->mvol;
    g_SndCurrentAttr.master_pan = program_attr->mpan;
    g_SndCurrentAttr.tones = program_attr->tones;

    tone_attr =
        &g_SndCurrentToneTable[g_SndCurrentAttr.tone +
                     g_SndCurrentAttr.program_index * 0x10];
    g_SndCurrentAttr.priority = tone_attr->prior;
    vag = tone_attr->vag;
    g_SndCurrentAttr.vag = vag;
    g_SndCurrentAttr.tone_volume = tone_attr->vol;
    g_SndCurrentAttr.tone_pan = tone_attr->pan;
    g_SndCurrentAttr.center = tone_attr->center;
    g_SndCurrentAttr.shift = tone_attr->shift;
    g_SndCurrentAttr.mode = tone_attr->mode;
    g_SndCurrentAttr.min = tone_attr->min;
    g_SndCurrentAttr.max = tone_attr->max;

    if (vag == 0) {
        g_SndUpdateLock = 0;
        return -1;
    }

    idx = (short)voice;
    g_SndCurrentAttr.voice = voice;
    g_SndVoiceState[idx].seq_sep = 0x21;
    g_SndVoiceState[idx].vab_id = vab_id;
    g_SndVoiceState[idx].program_index = g_SndCurrentAttr.program_index;
    g_SndVoiceState[idx].program = program;
    g_SndVoiceState[idx].vag = g_SndCurrentAttr.vag;
    tone_value = g_SndCurrentAttr.tone;
    g_SndVoiceState[idx].note = note;
    g_SndVoiceState[idx].active = 1;
    g_SndVoiceState[idx].age = 0;
    g_SndVoiceState[idx].tone = tone_value;
    SpuVmRebuildVoiceTable();
    if ((short)g_SndCurrentAttr.vag == 0xFF) {
        SpuVmNoiseKeyOn((u8)voice);
    } else {
        SpuVmScaleVabVolume(1, (u_short)SpuVmCalculateTonePitch(note, fine));
    }
    g_SndUpdateLock = 0;
    return (short)voice;
}
