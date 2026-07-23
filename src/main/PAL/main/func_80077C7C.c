#include "common.h"

typedef struct {
    u8 tones;
    u8 mvol;
    u8 prior;
    u8 mode;
    u8 mpan;
    u8 reserved0;
    s16 attr;
    u32 reserved1;
    u16 reserved2;
    u16 reserved3;
} ProgAtr77C7C;

typedef struct {
    u8 prior;
    u8 mode;
    u8 vol;
    u8 pan;
    u8 center;
    u8 shift;
    u8 min;
    u8 max;
    u8 vibW;
    u8 vibT;
    u8 porW;
    u8 porT;
    u8 pbmin;
    u8 pbmax;
    u8 reserved1;
    u8 reserved2;
    u16 adsr1;
    u16 adsr2;
    s16 prog;
    s16 vag;
    s16 reserved[4];
} VagAtr77C7C;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk04;
    u16 unk6;
    s16 unk8;
    u8 unka;
    u8 unkb;
    s16 note;
    s16 unke;
    s16 unk10;
    s16 prog;
    s16 tone;
    s16 vabId;
    s16 unk18;
    u8 pad19[1];
    u8 unk1b;
    s16 auto_vol;
    s16 unk1e;
    s16 unk20;
    s16 unk22;
    s16 start_vol;
    s16 end_vol;
    s16 auto_pan;
    s16 unk2a;
    s16 unk2c;
    s16 unk2e;
    s16 start_pan;
    s16 end_pan;
} SpuVoice77C7C;

typedef struct {
    u8 tones;
    u8 vab_id;
    u8 note;
    u8 fine;
    u8 volume;
    u8 pan;
    u8 program;
    u8 fake_program;
    u8 unk8;
    u8 unk9;
    u8 mvol;
    u8 mpan;
    u8 tone;
    u8 tone_volume;
    u8 tone_pan;
    u8 priority;
    u8 center;
    u8 shift;
    u8 min;
    u8 max;
    u8 mode;
    u8 pad15;
    u16 seq_sep;
    u16 vag;
    u16 voice;
    u16 unk1c;
    u16 unk1e;
} SvmCur77C7C;

extern s32 D_801E40AC;
extern ProgAtr77C7C *D_801E4110;
extern VagAtr77C7C *D_801E416C;
extern SvmCur77C7C D_801E4BD0;
extern SpuVoice77C7C D_8009E0B8[];

extern s32 func_80073314(s16, s16);
extern void func_80074134(void);
extern void func_80074348(s32);
extern s32 func_80074A6C(u16, u16);
extern void func_80073C50(s32, u16);

s32 func_80077C7C(
    s32 voice,
    s16 vab_id,
    s16 program,
    s16 tone,
    u16 note,
    u16 fine,
    s16 left,
    s16 right) {
    s32 idx;
    u8 tone_value;
    u16 vag;
    ProgAtr77C7C *program_attr;
    VagAtr77C7C *tone_attr;
    s32 left_value;
    s32 right_value;

    if (D_801E40AC == 1) {
        return -1;
    }
    D_801E40AC = 1;
    if ((u16)voice >= 24) {
        D_801E40AC = 0;
        return -1;
    }
    if (func_80073314(vab_id, program) != 0) {
        D_801E40AC = 0;
        return -1;
    }
    D_801E4BD0.seq_sep = 0x21;
    left_value = left;
    right_value = right;
    D_801E4BD0.note = note;
    D_801E4BD0.fine = fine;
    D_801E4BD0.tone = tone;

    if (left_value == right_value) {
        D_801E4BD0.pan = 0x40;
        D_801E4BD0.volume = left;
    } else if (right_value < left_value) {
        D_801E4BD0.volume = left;
        D_801E4BD0.pan = (right_value * 0x40) / left_value;
    } else {
        D_801E4BD0.volume = right;
        D_801E4BD0.pan =
            0x7F - ((left_value * 0x40) / right_value);
    }

    program_attr = &D_801E4110[program];
    D_801E4BD0.mvol = program_attr->mvol;
    D_801E4BD0.mpan = program_attr->mpan;
    D_801E4BD0.tones = program_attr->tones;

    tone_attr =
        &D_801E416C[D_801E4BD0.tone +
                     D_801E4BD0.fake_program * 0x10];
    D_801E4BD0.priority = tone_attr->prior;
    vag = tone_attr->vag;
    D_801E4BD0.vag = vag;
    D_801E4BD0.tone_volume = tone_attr->vol;
    D_801E4BD0.tone_pan = tone_attr->pan;
    D_801E4BD0.center = tone_attr->center;
    D_801E4BD0.shift = tone_attr->shift;
    D_801E4BD0.mode = tone_attr->mode;
    D_801E4BD0.min = tone_attr->min;
    D_801E4BD0.max = tone_attr->max;

    if (vag == 0) {
        D_801E40AC = 0;
        return -1;
    }

    idx = (s16)voice;
    D_801E4BD0.voice = voice;
    D_8009E0B8[idx].unke = 0x21;
    D_8009E0B8[idx].vabId = vab_id;
    D_8009E0B8[idx].unk10 = D_801E4BD0.fake_program;
    D_8009E0B8[idx].prog = program;
    D_8009E0B8[idx].unk0 = D_801E4BD0.vag;
    tone_value = D_801E4BD0.tone;
    D_8009E0B8[idx].note = note;
    D_8009E0B8[idx].unk1b = 1;
    D_8009E0B8[idx].unk2 = 0;
    D_8009E0B8[idx].tone = tone_value;
    func_80074134();
    if ((s16)D_801E4BD0.vag == 0xFF) {
        func_80074348(voice & 0xFF);
    } else {
        func_80073C50(1, func_80074A6C(note, fine));
    }
    D_801E40AC = 0;
    return (s16)voice;
}
