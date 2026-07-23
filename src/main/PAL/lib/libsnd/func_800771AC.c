#include "common.h"

typedef struct SeqState771AC {
    u8 unk0;
    u8 pad1[3];
    u8 *read_pos;
    u8 *next_sep_pos;
    u8 *loop_pos;
    u8 unk10;
    u8 unk11;
    u8 channel;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 panpot[16];
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 programs[16];
    u8 unk3C;
    u8 pad3D;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 vol[16];
    s16 unk6E;
    s16 unk70;
    s16 unk72;
    u16 unk74;
    u16 unk76;
    s16 unk78;
    s16 unk7A;
    s32 unk7C;
    u32 unk80;
    s32 unk84;
    s32 delta_value;
    s32 unk8C;
    s32 unk90;
    u32 unk94;
    u32 unk98;
    s32 unk9C;
    u32 unkA0;
    u32 unkA4;
    s16 padA8;
    s16 padAA;
} SeqState771AC;

typedef struct ProgAttr771AC {
    u8 tones;
    u8 mvol;
    u8 prior;
    u8 mode;
    u8 mpan;
    s8 reserved0;
    s16 attr;
    u32 reserved1;
    u16 reserved2;
    u16 reserved3;
} ProgAttr771AC;

typedef struct ToneAttr771AC {
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
} ToneAttr771AC;

typedef struct VabHeader771AC {
    s32 form;
    s32 ver;
    s32 id;
    u32 fsize;
    u16 reserved0;
    u16 ps;
    u16 ts;
    u16 vs;
    u8 mvol;
    u8 pan;
    u8 attr1;
    u8 attr2;
    u32 reserved1;
} VabHeader771AC;

typedef struct VoiceState771AC {
    s16 vag;
    s16 age;
    s16 pitch;
    u16 unk6;
    s16 base_volume;
    s8 unkA;
    s8 unkB;
    s16 note;
    s16 seq_sep;
    s16 program_index;
    s16 program;
    s16 tone;
    s16 vab_id;
    s16 priority;
    u8 pad1A;
    u8 status;
    s16 auto_volume;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 start_volume;
    s16 end_volume;
    s16 auto_pan;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 start_pan;
    s16 end_pan;
} VoiceState771AC;

typedef struct SvmCurrent771AC {
    u8 tones;
    u8 unk1;
    u8 note;
    u8 fine;
    u8 volume;
    u8 pan;
    u8 unk6;
    u8 fake_program;
    u8 unk8;
    u8 unk9;
    u8 master_volume;
    u8 master_pan;
    u8 tone;
    u8 tone_volume;
    u8 tone_pan;
    u8 priority;
    u8 center;
    u8 shift;
    u8 min;
    u8 max;
    u8 mode;
    u8 unk15;
    u16 seq_sep;
    s16 vag;
    u16 voice;
} SvmCurrent771AC;

extern SeqState771AC *D_801E79CC[];
extern VoiceState771AC D_8009E0B8[];
extern s16 D_8009DF20[];
extern u8 D_8009E0A0[];
extern u8 D_801E42F8;
extern s16 D_801E3FB0;
extern s16 D_801E4BE6;
extern ProgAttr771AC *D_801E4110;
extern VabHeader771AC *D_801E413C;
extern ToneAttr771AC *D_801E416C;
extern s32 D_801E40AC;
extern SvmCurrent771AC D_801E4BD0;

s32 func_80073314(s16 vab_id, s16 program);
u8 func_800739E8(s32 priority);
void func_80074134(void);
void func_80074348(s32 voice);
s32 func_80074A6C(u16 note, u16 fine);
void func_80073C50(s32 count, s32 pitch);

s32 func_800771AC(s16 seq_sep, s16 vab_id, s16 program, u16 volume, u16 pan) {
    SeqState771AC *score =
        &D_801E79CC[seq_sep & 0xFF][(seq_sep & 0xFF00) >> 8];
    s32 voices_updated;
    u8 voice;
    s32 base_volume;
    u32 left;
    u32 right;
    u32 left_scaled;
    u32 right_scaled;
    s32 master_volume;
    u8 current_pan;
    s16 current_program;

    voices_updated = 0;
    func_80073314(vab_id, program);
    D_801E4BE6 = seq_sep;
    for (voice = 0; voice < D_801E42F8; voice++) {
        current_program = program;
        if ((D_8009E0B8[voice].seq_sep == seq_sep) &&
            (D_8009E0B8[voice].program == current_program) &&
            (D_8009E0B8[voice].vab_id == vab_id)) {
            if (score->vol[score->channel] != volume &&
                score->vol[score->channel] == 0) {
                score->vol[score->channel] = 1;
            }
            base_volume = (D_8009E0B8[voice].base_volume * volume) / 0x7F;
            master_volume = D_801E413C->mvol * 0x3FFF;
            left_scaled = ((base_volume * master_volume) / 0x7F) / 0x7F;
            left_scaled =
                ((left_scaled * D_801E4110[current_program].mvol *
                  D_801E416C[D_8009E0B8[voice].tone +
                              (current_program * 0x10)]
                      .vol) /
                 0x7F) /
                0x7F;
            right_scaled = left_scaled;

            left_scaled = (left_scaled * score->unk74) / 0x7F;
            right_scaled = (right_scaled * score->unk76) / 0x7F;

            current_pan = D_801E416C[D_8009E0B8[voice].tone].pan;
            if (current_pan < 0x40) {
                left = left_scaled;
                right = (right_scaled * current_pan) / 0x3F;
            } else {
                left = (left_scaled * (0x7F - current_pan)) / 0x3F;
                right = right_scaled;
            }
            current_pan = D_801E4110[D_8009E0B8[voice].program_index].mpan;
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
            if (D_801E3FB0 == 1) {
                if (left < right) {
                    left = right;
                } else {
                    right = left;
                }
            }
            left = (left * left) / 0x3FFF;
            right = (right * right) / 0x3FFF;
            D_8009DF20[voice * 8] = left;
            D_8009DF20[voice * 8 + 1] = right;
            D_8009E0A0[voice] |= 3;
            voices_updated++;
        }
    }
    return voices_updated;
}

s16 func_800776E4(
    s16 vab_id,
    s16 program,
    s16 tone,
    s16 note,
    s16 fine,
    s16 left,
    s16 right) {
    u16 voice;
    s16 voice_index;
    s16 vag;
    s32 tone_index;
    s32 left_value;
    s32 right_value;

    if (D_801E40AC == 1) {
        return -1;
    }
    D_801E40AC = 1;

    if (func_80073314(vab_id, program)) {
        D_801E40AC = 0;
        return -1;
    }
    D_801E4BD0.seq_sep = 0x21;
    D_801E4BD0.note = note;
    D_801E4BD0.fine = fine;
    D_801E4BD0.tone = tone;

    left_value = left;
    right_value = right;
    if (left_value == right_value) {
        D_801E4BD0.pan = 0x40;
        D_801E4BD0.volume = left;
    } else if (right_value < left_value) {
        D_801E4BD0.volume = left;
        D_801E4BD0.pan = (right_value * 0x40) / left_value;
    } else {
        D_801E4BD0.volume = right;
        D_801E4BD0.pan = 0x7F - ((left_value * 0x40) / right_value);
    }

    D_801E4BD0.master_volume = D_801E4110[program].mvol;
    D_801E4BD0.master_pan = D_801E4110[program].mpan;
    D_801E4BD0.tones = D_801E4110[program].tones;

    tone_index = D_801E4BD0.tone + (D_801E4BD0.fake_program * 0x10);
    D_801E4BD0.priority = D_801E416C[tone_index].prior;
    vag = D_801E416C[tone_index].vag;
    D_801E4BD0.vag = vag;
    D_801E4BD0.tone_volume = D_801E416C[tone_index].vol;
    D_801E4BD0.tone_pan = D_801E416C[tone_index].pan;
    D_801E4BD0.center = D_801E416C[tone_index].center;
    D_801E4BD0.shift = D_801E416C[tone_index].shift;
    D_801E4BD0.mode = D_801E416C[tone_index].mode;
    D_801E4BD0.min = D_801E416C[tone_index].min;
    D_801E4BD0.max = D_801E416C[tone_index].max;

    if (vag == 0) {
        D_801E40AC = 0;
        return -1;
    }

    voice = func_800739E8(vag);
    voice_index = voice;
    if (voice_index == D_801E42F8) {
        D_801E40AC = 0;
        return -1;
    }

    D_801E4BD0.voice = voice;
    D_8009E0B8[voice_index].seq_sep = 0x21;
    D_8009E0B8[voice_index].vab_id = vab_id;
    D_8009E0B8[voice_index].program_index = D_801E4BD0.fake_program;
    D_8009E0B8[voice_index].program = program;
    D_8009E0B8[voice_index].vag = D_801E4BD0.vag;
    D_8009E0B8[voice_index].tone = D_801E4BD0.tone;
    D_8009E0B8[voice_index].note = note;
    D_8009E0B8[voice_index].status = 1;
    D_8009E0B8[voice_index].age = 0;

    func_80074134();
    if ((s16)D_801E4BD0.vag == 0xFF) {
        func_80074348(voice);
    } else {
        func_80073C50(1, func_80074A6C(note, fine) & 0xFFFF);
    }
    D_801E40AC = 0;
    return voice;
}
