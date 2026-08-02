#include <sys/types.h>

#include "common.h"

typedef struct ProgAtr77C7C {
    u_char tones;
    u_char mvol;
    u_char prior;
    u_char mode;
    u_char mpan;
    u_char reserved0;
    short attr;
    u_long reserved1;
    u_short reserved2;
    u_short reserved3;
} ProgAtr77C7C;

typedef struct VagAtr77C7C {
    u_char prior;
    u_char mode;
    u_char vol;
    u_char pan;
    u_char center;
    u_char shift;
    u_char min;
    u_char max;
    u_char vibW;
    u_char vibT;
    u_char porW;
    u_char porT;
    u_char pbmin;
    u_char pbmax;
    u_char reserved1;
    u_char reserved2;
    u_short adsr1;
    u_short adsr2;
    short prog;
    short vag;
    short reserved[4];
} VagAtr77C7C;

typedef struct SpuVoice77C7C {
    short unk0;
    short unk2;
    short unk04;
    u_short unk6;
    short unk8;
    u_char unka;
    u_char unkb;
    short note;
    short unke;
    short unk10;
    short prog;
    short tone;
    short vabId;
    short unk18;
    u_char pad19[1];
    u_char unk1b;
    short auto_vol;
    short unk1e;
    short unk20;
    short unk22;
    short start_vol;
    short end_vol;
    short auto_pan;
    short unk2a;
    short unk2c;
    short unk2e;
    short start_pan;
    short end_pan;
} SpuVoice77C7C;

typedef struct SvmCur77C7C {
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
    u_char mvol;
    u_char mpan;
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
    u_short vag;
    u_short voice;
    u_short unk1c;
    u_short unk1e;
} SvmCur77C7C;

extern long g_SndUpdateLock asm("D_801E40AC");
extern ProgAtr77C7C *g_SndCurrentProgTable asm("D_801E4110");
extern VagAtr77C7C *g_SndCurrentToneTable asm("D_801E416C");
extern SvmCur77C7C g_SndCurrentAttr asm("D_801E4BD0");
extern SpuVoice77C7C g_SndVoiceState[] asm("D_8009E0B8");

extern long SpuVmVSetUp(short, short) asm("func_80073314");
extern void func_80074134(void);
extern void SpuVmNoiseKeyOn(long) asm("func_80074348");
extern long SpuVmCalculateTonePitch(u_short, u_short) asm("func_80074A6C");
extern void func_80073C50(long, u_short);

long SsUtKeyOnV(
    long voice,
    short vab_id,
    short program,
    short tone,
    u_short note,
    u_short fine,
    short left,
    short right) asm("func_80077C7C");

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
    ProgAtr77C7C *program_attr;
    VagAtr77C7C *tone_attr;
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
    g_SndCurrentAttr.mvol = program_attr->mvol;
    g_SndCurrentAttr.mpan = program_attr->mpan;
    g_SndCurrentAttr.tones = program_attr->tones;

    tone_attr =
        &g_SndCurrentToneTable[g_SndCurrentAttr.tone +
                     g_SndCurrentAttr.fake_program * 0x10];
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
    g_SndVoiceState[idx].unke = 0x21;
    g_SndVoiceState[idx].vabId = vab_id;
    g_SndVoiceState[idx].unk10 = g_SndCurrentAttr.fake_program;
    g_SndVoiceState[idx].prog = program;
    g_SndVoiceState[idx].unk0 = g_SndCurrentAttr.vag;
    tone_value = g_SndCurrentAttr.tone;
    g_SndVoiceState[idx].note = note;
    g_SndVoiceState[idx].unk1b = 1;
    g_SndVoiceState[idx].unk2 = 0;
    g_SndVoiceState[idx].tone = tone_value;
    func_80074134();
    if ((short)g_SndCurrentAttr.vag == 0xFF) {
        SpuVmNoiseKeyOn(voice & 0xFF);
    } else {
        func_80073C50(1, SpuVmCalculateTonePitch(note, fine));
    }
    g_SndUpdateLock = 0;
    return (short)voice;
}
