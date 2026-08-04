#include "common.h"

/*
 * ContDataEntry - the real libsnd name, recovered from Runtime Library 2.6's
 * LIBSND.LIB export table, where SEQREAD.C's helpers are still separate symbols
 * because that build did not inline them. See docs/LIBSND_NAMES_FROM_RL26.md.
 * Reached from SsSeqDispatchControlChange's `case 6:`. Applies the pending
 * RPN (unk29 == 2) or NRPN (unk2a == 2) to the channel's VAB program by
 * rewriting the VagAtr of every tone. See docs/names.md 17.
 */
void ContDataEntry(s16 seq, s16 sep, u8 value) asm("func_8007010C");

typedef struct SeqStruct {
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
    u8 unk2a;
    u8 unk2b;
    u8 programs[16];
    u8 unk3c;
    u8 pad3d;
    s16 unk3e;
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4a;
    s16 unk4c;
    s16 vol[16];
    s16 unk6e;
    s16 unk70;
    s16 unk72;
    u16 unk74;
    u16 unk76;
    s16 unk78;
    s16 unk7a;
    s32 unk7c;
    u32 unk80;
    s32 unk84;
    s32 delta_value;
    s32 unk8c;
    s32 unk90;
    u32 unk94;
    u32 unk98;
    s32 unk9c;
    u32 unka0;
    u32 unka4;
    s16 pada8;
    s16 padaa;
} SeqStruct;

typedef struct ProgAtr {
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
} ProgAtr;

typedef struct VagAtr {
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
} VagAtr;

struct Unk {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    s16 unkA;
    s16 unkC;
    u16 unkE;
    s16 unk10;
};

extern SeqStruct *D_801E79CC[];

s32 SsSeqReadDeltaTime(s32 seq, s16 sep) asm("func_80070D70");
s16 SsUtGetProgAtr(s16 vab_id, s16 program, ProgAtr *out) asm("func_8007320C");
s16 SsUtGetVagAtr(s16 vab_id, s16 program, s16 tone, VagAtr *out) asm("func_800733D8");
s16 SsUtSetVagAtr(s16 vab_id, s16 program, s16 tone, VagAtr *in) asm("func_80073820");
void SsSeqApplyNrpn(
    s16 vab_id,
    s16 program,
    s16 tone,
    VagAtr tone_attr,
    struct Unk adsr,
    s16 index,
    u8 value) asm("func_800706AC");

static inline s32 SsSeqCheckDataEntryValue(s32 data_entry_value) {
    switch (data_entry_value) {
    case 0:
        return 0;
    case 10:
        return 0;
    default:
        return 0;
    }
}

void ContDataEntry(s16 seq, s16 sep, u8 value) {
    s32 mask;
    ProgAtr program_attr;
    VagAtr tone_attr;
    struct Unk adsr;
    SeqStruct *score;
    s32 tone;
    u8 channel;

    mask = 0xFF;
    score = &D_801E79CC[seq][sep];
    channel = score->channel;
    SsUtGetProgAtr(score->unk4c, score->programs[channel], &program_attr);
    if (score->unk27 == 1 && score->unk10 == 0) {
        score->unk28 = value;
        score->unk10 = 1;
        score->delta_value = SsSeqReadDeltaTime(seq, sep);
    } else if (score->unk29 == 2) {
        if (score->unk13 == 0 && score->unk14 == 0) {
            tone = 0;
            if (tone < (program_attr.tones + value) - value) {
                do {
                    s32 bend = value & 0x7F;

                    SsUtGetVagAtr(score->unk4c, score->programs[channel], tone,
                                 &tone_attr);
                    tone_attr.pbmin = tone_attr.pbmax = bend;
                    SsUtSetVagAtr(score->unk4c, score->programs[channel], tone,
                                 &tone_attr);
                    tone++;
                } while (tone < program_attr.tones);
            }
        }
        if (score->unk13 == 1 && score->unk14 == 0) {
            s32 shift;

            if (value > 0x40 && value < 0x80) {
                shift = SsSeqCheckDataEntryValue(value);
                shift =
                    ((((value) * 100) / 0x2000) * 0x2000) & 0xE000;
            } else {
                shift = 0;
            }
            tone = 0;
            if (tone < (program_attr.tones + value) - value) {
                do {
                    SsUtGetVagAtr(
                        score->unk4c, score->programs[channel], tone, &tone_attr);
                    tone_attr.shift += shift;
                    SsUtSetVagAtr(
                        score->unk4c, score->programs[channel], tone, &tone_attr);
                    tone++;
                } while (tone < program_attr.tones);
            }
        }
        if (score->unk13 == 2 && score->unk14 == 0) {
            s32 center;

            if (value >= 0x40 && value < 0x80) {
                center = (value * 100) * 0x40;
            } else {
                center = 0;
            }
            tone = 0;
            if (tone < (program_attr.tones + value) - value) {
                do {
                    SsUtGetVagAtr(
                        score->unk4c, score->programs[channel], tone, &tone_attr);
                    tone_attr.center += center;
                    SsUtSetVagAtr(
                        score->unk4c, score->programs[channel], tone, &tone_attr);
                    tone++;
                } while (tone < program_attr.tones);
            }
        }
        score->delta_value = SsSeqReadDeltaTime(seq, sep);
        score->unk29 = 0;
    } else if (score->unk2a == 2) {
        if (score->unk16 == 0x10) {
            for (tone = 0; tone < program_attr.tones; tone++) {
                SsSeqApplyNrpn(
                    score->unk4c,
                    score->programs[channel],
                    tone,
                    tone_attr,
                    adsr,
                    score->unk15,
                    value);
            }
        } else {
            SsSeqApplyNrpn(
                score->unk4c,
                score->programs[channel],
                score->unk16,
                tone_attr,
                adsr,
                score->unk15,
                value);
        }
        score->delta_value = SsSeqReadDeltaTime(seq, sep);
        score->unk2a = 0;
    } else {
        score->delta_value = SsSeqReadDeltaTime(seq, sep);
    }
}
