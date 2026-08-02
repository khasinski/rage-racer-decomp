#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

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

extern SeqState76350 *g_SndSeqTable[] asm("D_801E79CC");
extern ProgAtr *g_SndCurrentProgTable asm("D_801E4110");
extern VabHdr *g_SndCurrentVabHeader asm("D_801E413C");
extern VagAtr *g_SndCurrentToneTable asm("D_801E416C");
extern u_char D_801E42F8;
extern SvmCurrent76350 g_SndCurrentAttr asm("D_801E4BD0");
extern SpuVoice g_SndVoiceState[] asm("D_8009E0B8");

long SpuVmVSetUp(short vab_id, short program) asm("func_80073314");
long SpuVmSeKeyOff(short seq_sep, short vab_id, short program, u_short note) asm("func_80076940");
u_char SpuVmAlloc(long priority) asm("func_800739E8");
void func_80074134(void);
void SpuVmNoiseKeyOn(u_char voice) asm("func_80074348");
u_short SpuVmCalculateCurrentPitch(void) asm("func_800749B4");
void func_80073C50(u_char tone_count, u_short pitch);

static inline u_char func_80076350_select_tones(
    u_char *tone_indices, u_char *vag_indices) {
    u_char tone;
    u_char count;
    VagAtr *attr;

    count = 0;
    for (tone = 0; tone < g_SndCurrentAttr.tone_count; tone++) {
        attr =
            &g_SndCurrentToneTable[(g_SndCurrentAttr.program_index * 0x10) + tone];
        if (attr->min > g_SndCurrentAttr.note ||
            g_SndCurrentAttr.note > attr->max) {
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
        &g_SndSeqTable[seq_sep & 0xFF][(seq_sep & 0xFF00) >> 8];
    u_char vag_indices[0x80];
    u_char tone_indices[0x80];
    u_char i;
    long result;
    long tone;
    u_char tone_count;

    result = 0;
    if (SpuVmVSetUp(vab_id, program)) {
        return -1;
    }
    g_SndCurrentAttr.seq_sep = seq_sep;
    g_SndCurrentAttr.note = note;
    g_SndCurrentAttr.unk3 = 0;
    if (seq_sep == 0x21) {
        g_SndCurrentAttr.volume = volume;
    } else {
        g_SndCurrentAttr.volume =
            (volume * score->vol[score->channel]) / 0x7F;
    }

    g_SndCurrentAttr.pan = pan;
    g_SndCurrentAttr.master_volume = g_SndCurrentProgTable[program].mvol;
    g_SndCurrentAttr.master_pan = g_SndCurrentProgTable[program].mpan;
    g_SndCurrentAttr.tone_count = g_SndCurrentProgTable[program].tones;
    if (g_SndCurrentAttr.program_index >= g_SndCurrentVabHeader->ps) {
        return -1;
    }
    if (volume == 0) {
        result = SpuVmSeKeyOff(seq_sep, vab_id, program, note);
    } else {
        tone_count =
            func_80076350_select_tones(tone_indices, vag_indices);
        for (i = 0; i < tone_count; i++) {
            g_SndCurrentAttr.vag = vag_indices[i];
            g_SndCurrentAttr.tone = tone_indices[i];

            tone =
                g_SndCurrentAttr.tone +
                (g_SndCurrentAttr.program_index * 0x10);
            g_SndCurrentAttr.priority = g_SndCurrentToneTable[tone].prior;
            g_SndCurrentAttr.tone_volume = g_SndCurrentToneTable[tone].vol;
            g_SndCurrentAttr.tone_pan = g_SndCurrentToneTable[tone].pan;
            g_SndCurrentAttr.center = g_SndCurrentToneTable[tone].center;
            g_SndCurrentAttr.shift = g_SndCurrentToneTable[tone].shift;
            g_SndCurrentAttr.mode = g_SndCurrentToneTable[tone].mode;
            g_SndCurrentAttr.min = g_SndCurrentToneTable[tone].min;
            g_SndCurrentAttr.max = g_SndCurrentToneTable[tone].max;
            g_SndCurrentAttr.voice = SpuVmAlloc(0);
            if (g_SndCurrentAttr.voice < D_801E42F8) {
                g_SndVoiceState[g_SndCurrentAttr.voice].active = 1;
                g_SndVoiceState[g_SndCurrentAttr.voice].age = 0;
                g_SndVoiceState[g_SndCurrentAttr.voice].seq_sep = seq_sep;
                g_SndVoiceState[g_SndCurrentAttr.voice].vab_id =
                    g_SndCurrentAttr.vab_id;
                g_SndVoiceState[g_SndCurrentAttr.voice].program_index =
                    g_SndCurrentAttr.program_index;
                g_SndVoiceState[g_SndCurrentAttr.voice].program = program;
                if (seq_sep != 0x21) {
                    g_SndVoiceState[g_SndCurrentAttr.voice].base_volume =
                        volume;
                }
                g_SndVoiceState[g_SndCurrentAttr.voice].pan = pan;
                g_SndVoiceState[g_SndCurrentAttr.voice].tone =
                    g_SndCurrentAttr.tone;
                g_SndVoiceState[g_SndCurrentAttr.voice].note = note;
                g_SndVoiceState[g_SndCurrentAttr.voice].priority =
                    g_SndCurrentAttr.priority;
                g_SndVoiceState[g_SndCurrentAttr.voice].vag =
                    g_SndCurrentAttr.vag;
                func_80074134();
                if (g_SndCurrentAttr.vag == 0xFF) {
                    SpuVmNoiseKeyOn(g_SndCurrentAttr.voice);
                } else {
                    func_80073C50(
                        tone_count, SpuVmCalculateCurrentPitch() & 0xFFFF);
                }
                result |= 1 << g_SndCurrentAttr.voice;
            } else {
                result = -1;
            }
        }
    }
    return result;
}
