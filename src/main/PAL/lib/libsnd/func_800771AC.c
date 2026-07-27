#include <sys/types.h>

#include "common.h"

typedef struct SeqState771AC {
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
    long delta_value;
    long unk8C;
    long unk90;
    u_long unk94;
    u_long unk98;
    long unk9C;
    u_long unkA0;
    u_long unkA4;
    short padA8;
    short padAA;
} SeqState771AC;

typedef struct ProgAttr771AC {
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
} ProgAttr771AC;

typedef struct ToneAttr771AC {
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
} ToneAttr771AC;

typedef struct VabHeader771AC {
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
} VabHeader771AC;

typedef struct VoiceState771AC {
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
} VoiceState771AC;

typedef struct SvmCurrent771AC {
    u_char tones;
    u_char unk1;
    u_char note;
    u_char fine;
    u_char volume;
    u_char pan;
    u_char unk6;
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
    u_char unk15;
    u_short seq_sep;
    short vag;
    u_short voice;
} SvmCurrent771AC;

extern SeqState771AC *D_801E79CC[];
extern VoiceState771AC D_8009E0B8[];
extern short D_8009DF20[];
extern u_char D_8009E0A0[];
extern u_char D_801E42F8;
extern short D_801E3FB0;
extern short D_801E4BE6;
extern ProgAttr771AC *D_801E4110;
extern VabHeader771AC *D_801E413C;
extern ToneAttr771AC *D_801E416C;
extern long D_801E40AC;
extern SvmCurrent771AC D_801E4BD0;

long func_80073314(short vab_id, short program);
u_char func_800739E8(long priority);
void func_80074134(void);
void SpuVmNoiseKeyOn(long voice) asm("func_80074348");
long func_80074A6C(u_short note, u_short fine);
void func_80073C50(long count, long pitch);

long func_800771AC(short seq_sep, short vab_id, short program, u_short volume, u_short pan) {
    SeqState771AC *score =
        &D_801E79CC[seq_sep & 0xFF][(seq_sep & 0xFF00) >> 8];
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

short SsUtKeyOn(short, short, short, short, short, short, short) asm("func_800776E4");
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
    if ((short)D_801E4BD0.vag == 0xFF) {
        SpuVmNoiseKeyOn(voice);
    } else {
        func_80073C50(1, func_80074A6C(note, fine) & 0xFFFF);
    }
    D_801E40AC = 0;
    return voice;
}
