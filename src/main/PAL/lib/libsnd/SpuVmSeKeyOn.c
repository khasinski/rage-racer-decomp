#include <sys/types.h>

#include "common.h"

typedef struct SeqState76350 {
    u_char unk0;
    u_char pad1[3];
    u_char *read_pos;
    u_char *next_sep_pos;
    u_char *loop_pos;
    u_char unk10;
    u_char unk11;
    u_char channel;
    u_char unk13;
    u_char unk14;
    u_char unk15;
    u_char unk16;
    u_char panpot[16];
    u_char unk27;
    u_char unk28;
    u_char unk29;
    u_char unk2A;
    u_char unk2B;
    u_char programs[16];
    u_char unk3C;
    u_char pad3D;
    short unk3E;
    short unk40;
    short unk42;
    short unk44;
    short unk46;
    short unk48;
    short unk4A;
    short unk4C;
    short vol[16];
    short unk6E;
    short unk70;
    short unk72;
    u_short unk74;
    u_short unk76;
    short unk78;
    short unk7A;
    long unk7C;
    u_long unk80;
    long unk84;
    long unk88;
    long unk8C;
    long unk90;
    u_long unk94;
    u_long unk98;
    long unk9C;
    u_long unkA0;
    u_long unkA4;
    short padA8;
    short padAA;
} SeqState76350;

typedef struct VabHeader76350 {
    long form;
    long ver;
    long id;
    u_long fsize;
    u_short reserved0;
    u_short ps;
    u_short ts;
    u_short vs;
    u_char mvol;
    u_char pan;
    u_char attr1;
    u_char attr2;
    u_long reserved1;
} VabHeader76350;

typedef struct ProgAttr76350 {
    u_char tones;
    u_char mvol;
    u_char prior;
    u_char mode;
    u_char mpan;
    signed char reserved0;
    short attr;
    u_long reserved1;
    u_short reserved2;
    u_short reserved3;
} ProgAttr76350;

typedef struct ToneAttr76350 {
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
} ToneAttr76350;

typedef struct VoiceState76350 {
    short vag;
    short age;
    short pitch;
    u_short env;
    short base_volume;
    signed char pan;
    signed char unkB;
    short note;
    short seq_sep;
    short program_index;
    short program;
    short tone;
    short vab_id;
    short priority;
    u_char pad1A;
    u_char active;
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
} VoiceState76350;

typedef struct SvmCurrent76350 {
    u_char tone_count;
    u_char vab_id;
    u_char note;
    u_char unk3;
    u_char volume;
    u_char pan;
    u_char program;
    u_char program_index;
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
    short seq_sep;
    short vag;
    short voice;
    short register_offset;
    short tone_index;
} SvmCurrent76350;

extern SeqState76350 *D_801E79CC[];
extern ProgAttr76350 *D_801E4110;
extern VabHeader76350 *D_801E413C;
extern ToneAttr76350 *D_801E416C;
extern u_char D_801E42F8;
extern SvmCurrent76350 D_801E4BD0;
extern VoiceState76350 D_8009E0B8[];

long func_80073314(short vab_id, short program);
long func_80076940(short seq_sep, short vab_id, short program, u_short note);
u_char func_800739E8(long priority);
void func_80074134(void);
void SpuVmNoiseKeyOn(u_char voice) asm("func_80074348");
u_short func_800749B4(void);
void func_80073C50(u_char tone_count, u_short pitch);

static inline u_char func_80076350_select_tones(
    u_char *tone_indices, u_char *vag_indices) {
    u_char tone;
    u_char count;
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

long SpuVmSeKeyOn(
    short seq_sep,
    short vab_id,
    short program,
    u_short note,
    u_short volume,
    u_short pan) asm("func_80076350");
long SpuVmSeKeyOn(
    short seq_sep,
    short vab_id,
    short program,
    u_short note,
    u_short volume,
    u_short pan) {
    SeqState76350 *score =
        &D_801E79CC[seq_sep & 0xFF][(seq_sep & 0xFF00) >> 8];
    u_char vag_indices[0x80];
    u_char tone_indices[0x80];
    u_char i;
    long result;
    long tone;
    u_char tone_count;

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
                    SpuVmNoiseKeyOn(D_801E4BD0.voice);
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
