#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

/* Not SvmCurrentAttr from psyq/snd_types.h, though the layout is identical:
 * this TU loads seq_sep, vag and voice signed, and folding it onto the
 * shared u_short spelling turns lh into lhu and stops matching. */
typedef struct SvmCurrentAttrKeyOn {
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
} SvmCurrentAttrKeyOn;

extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");
extern ProgAtr *g_SndCurrentProgTable;
extern VabHdr *g_SndCurrentVabHeader;
extern VagAtr *g_SndCurrentToneTable;
extern u_char g_SndVoiceCount;
extern SvmCurrentAttrKeyOn g_SndCurrentAttr asm("D_801E4BD0");
extern SpuVoice g_SndVoiceState[];

long SpuVmVSetUp(short vab_id, short program);
long SpuVmSeKeyOff(short seq_sep, short vab_id, short program, u_short note);
u_char SpuVmAlloc(long priority);
void SpuVmRebuildVoiceTable(void);
void SpuVmNoiseKeyOn(u_char voice);
u_short SpuVmCalculateCurrentPitch(void);
void SpuVmScaleVabVolume(u_char tone_count, u_short pitch);

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
    u_short pan);
long SpuVmSeKeyOn(
    short seq_sep,
    short vab_id,
    short program,
    u_short note,
    u_short volume,
    u_short pan) {
    SeqStruct *score =
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
            if (g_SndCurrentAttr.voice < g_SndVoiceCount) {
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
                SpuVmRebuildVoiceTable();
                if (g_SndCurrentAttr.vag == 0xFF) {
                    SpuVmNoiseKeyOn(g_SndCurrentAttr.voice);
                } else {
                    SpuVmScaleVabVolume(
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
