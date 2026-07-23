#include "common.h"

typedef struct SeqState76350 {
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
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    u32 unk94;
    u32 unk98;
    s32 unk9C;
    u32 unkA0;
    u32 unkA4;
    s16 padA8;
    s16 padAA;
} SeqState76350;

typedef struct VabHeader76350 {
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
} VabHeader76350;

typedef struct ProgAttr76350 {
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
} ProgAttr76350;

typedef struct ToneAttr76350 {
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
} ToneAttr76350;

typedef struct VoiceState76350 {
    s16 vag;
    s16 age;
    s16 pitch;
    u16 env;
    s16 base_volume;
    s8 pan;
    s8 unkB;
    s16 note;
    s16 seq_sep;
    s16 program_index;
    s16 program;
    s16 tone;
    s16 vab_id;
    s16 priority;
    u8 pad1A;
    u8 active;
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
} VoiceState76350;

typedef struct SvmCurrent76350 {
    u8 tone_count;
    u8 vab_id;
    u8 note;
    u8 unk3;
    u8 volume;
    u8 pan;
    u8 program;
    u8 program_index;
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
    u8 pad15;
    s16 seq_sep;
    s16 vag;
    s16 voice;
    s16 register_offset;
    s16 tone_index;
} SvmCurrent76350;

extern SeqState76350 *D_801E79CC[];
extern ProgAttr76350 *D_801E4110;
extern VabHeader76350 *D_801E413C;
extern ToneAttr76350 *D_801E416C;
extern u8 D_801E42F8;
extern SvmCurrent76350 D_801E4BD0;
extern VoiceState76350 D_8009E0B8[];

s32 func_80073314(s16 vab_id, s16 program);
s32 func_80076940(s16 seq_sep, s16 vab_id, s16 program, u16 note);
u8 func_800739E8(s32 priority);
void func_80074134(void);
void func_80074348(u8 voice);
u16 func_800749B4(void);
void func_80073C50(u8 tone_count, u16 pitch);

static inline u8 func_80076350_select_tones(
    u8 *tone_indices, u8 *vag_indices) {
    u8 tone;
    u8 count;
    ToneAttr76350 *attr;

    count = 0;
    for (tone = 0; tone < D_801E4BD0.tone_count; tone++) {
        attr =
            &D_801E416C[(D_801E4BD0.program_index * 0x10) + tone];
        if (attr->min > D_801E4BD0.note ||
            D_801E4BD0.note > attr->max) {
            continue;
        }
        vag_indices[count] = attr->vag;
        tone_indices[count++] = tone;
    }
    return count;
}

s32 func_80076350(
    s16 seq_sep,
    s16 vab_id,
    s16 program,
    u16 note,
    u16 volume,
    u16 pan) {
    SeqState76350 *score =
        &D_801E79CC[seq_sep & 0xFF][(seq_sep & 0xFF00) >> 8];
    u8 vag_indices[0x80];
    u8 tone_indices[0x80];
    u8 i;
    s32 result;
    s32 tone;
    u8 tone_count;

    result = 0;
    if (func_80073314(vab_id, program)) {
        return -1;
    }
    D_801E4BD0.seq_sep = seq_sep;
    D_801E4BD0.note = note;
    D_801E4BD0.unk3 = 0;
    if (seq_sep == 0x21) {
        D_801E4BD0.volume = volume;
    } else {
        D_801E4BD0.volume =
            (volume * score->vol[score->channel]) / 0x7F;
    }

    D_801E4BD0.pan = pan;
    D_801E4BD0.master_volume = D_801E4110[program].mvol;
    D_801E4BD0.master_pan = D_801E4110[program].mpan;
    D_801E4BD0.tone_count = D_801E4110[program].tones;
    if (D_801E4BD0.program_index >= D_801E413C->ps) {
        return -1;
    }
    if (volume == 0) {
        result = func_80076940(seq_sep, vab_id, program, note);
    } else {
        tone_count =
            func_80076350_select_tones(tone_indices, vag_indices);
        for (i = 0; i < tone_count; i++) {
            D_801E4BD0.vag = vag_indices[i];
            D_801E4BD0.tone = tone_indices[i];

            tone =
                D_801E4BD0.tone +
                (D_801E4BD0.program_index * 0x10);
            D_801E4BD0.priority = D_801E416C[tone].prior;
            D_801E4BD0.tone_volume = D_801E416C[tone].vol;
            D_801E4BD0.tone_pan = D_801E416C[tone].pan;
            D_801E4BD0.center = D_801E416C[tone].center;
            D_801E4BD0.shift = D_801E416C[tone].shift;
            D_801E4BD0.mode = D_801E416C[tone].mode;
            D_801E4BD0.min = D_801E416C[tone].min;
            D_801E4BD0.max = D_801E416C[tone].max;
            D_801E4BD0.voice = func_800739E8(0);
            if (D_801E4BD0.voice < D_801E42F8) {
                D_8009E0B8[D_801E4BD0.voice].active = 1;
                D_8009E0B8[D_801E4BD0.voice].age = 0;
                D_8009E0B8[D_801E4BD0.voice].seq_sep = seq_sep;
                D_8009E0B8[D_801E4BD0.voice].vab_id =
                    D_801E4BD0.vab_id;
                D_8009E0B8[D_801E4BD0.voice].program_index =
                    D_801E4BD0.program_index;
                D_8009E0B8[D_801E4BD0.voice].program = program;
                if (seq_sep != 0x21) {
                    D_8009E0B8[D_801E4BD0.voice].base_volume =
                        volume;
                }
                D_8009E0B8[D_801E4BD0.voice].pan = pan;
                D_8009E0B8[D_801E4BD0.voice].tone =
                    D_801E4BD0.tone;
                D_8009E0B8[D_801E4BD0.voice].note = note;
                D_8009E0B8[D_801E4BD0.voice].priority =
                    D_801E4BD0.priority;
                D_8009E0B8[D_801E4BD0.voice].vag =
                    D_801E4BD0.vag;
                func_80074134();
                if (D_801E4BD0.vag == 0xFF) {
                    func_80074348(D_801E4BD0.voice);
                } else {
                    func_80073C50(
                        tone_count, func_800749B4() & 0xFFFF);
                }
                result |= 1 << D_801E4BD0.voice;
            } else {
                result = -1;
            }
        }
    }
    return result;
}
