#include "psyq/snd.h"
#include "psyq/snd_internal.h"

/*
 * ContDataEntry - the real libsnd name, recovered from Runtime Library 2.6's
 * LIBSND.LIB export table, where SEQREAD.C's helpers are still separate symbols
 * because that build did not inline them. See docs/LIBSND_NAMES_FROM_RL26.md.
 * Reached from SsSeqDispatchControlChange's `case 6:`. Applies the pending
 * RPN (rpn_pending == 2) or NRPN (nrpn_pending == 2) to the channel's VAB program by
 * rewriting the VagAtr of every tone. See docs/names.md 17.
 */
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
    SndAdsr adsr;
    SeqStruct *score;
    s32 tone;
    u8 channel;

    mask = 0xFF;
    score = &g_SndSeqTable[seq][sep];
    channel = score->channel;
    SsUtGetProgAtr(score->vab_id, score->programs[channel], &program_attr);
    if (score->loop_marked == 1 && score->loop_count_set == 0) {
        score->loop_count = value;
        score->loop_count_set = 1;
        score->delta_value = SsSeqReadDeltaTime(seq, sep);
    } else if (score->rpn_pending == 2) {
        if (score->rpn_param == 0 && score->play_mode == 0) {
            tone = 0;
            if (tone < (program_attr.tones + value) - value) {
                do {
                    s32 bend = value & 0x7F;

                    SsUtGetVagAtr(score->vab_id, score->programs[channel], (s16)tone,
                                 &tone_attr);
                    tone_attr.pbmin = tone_attr.pbmax = bend;
                    SsUtSetVagAtr(score->vab_id, score->programs[channel], (s16)tone,
                                 &tone_attr);
                    tone++;
                } while (tone < program_attr.tones);
            }
        }
        if (score->rpn_param == 1 && score->play_mode == 0) {
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
                        score->vab_id, score->programs[channel], (s16)tone, &tone_attr);
                    tone_attr.shift += shift;
                    SsUtSetVagAtr(
                        score->vab_id, score->programs[channel], (s16)tone, &tone_attr);
                    tone++;
                } while (tone < program_attr.tones);
            }
        }
        if (score->rpn_param == 2 && score->play_mode == 0) {
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
                        score->vab_id, score->programs[channel], (s16)tone, &tone_attr);
                    tone_attr.center += center;
                    SsUtSetVagAtr(
                        score->vab_id, score->programs[channel], (s16)tone, &tone_attr);
                    tone++;
                } while (tone < program_attr.tones);
            }
        }
        score->delta_value = SsSeqReadDeltaTime(seq, sep);
        score->rpn_pending = 0;
    } else if (score->nrpn_pending == 2) {
        if (score->nrpn_msb == 0x10) {
            for (tone = 0; tone < program_attr.tones; tone++) {
                SsSeqApplyNrpn(
                    score->vab_id,
                    score->programs[channel],
                    tone,
                    tone_attr,
                    adsr,
                    score->nrpn_lsb,
                    value);
            }
        } else {
            SsSeqApplyNrpn(
                score->vab_id,
                score->programs[channel],
                score->nrpn_msb,
                tone_attr,
                adsr,
                score->nrpn_lsb,
                value);
        }
        score->delta_value = SsSeqReadDeltaTime(seq, sep);
        score->nrpn_pending = 0;
    } else {
        score->delta_value = SsSeqReadDeltaTime(seq, sep);
    }
}
