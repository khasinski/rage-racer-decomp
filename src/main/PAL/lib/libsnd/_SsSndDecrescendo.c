#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"
#include "psyq/snd_types.h"

#include "psyq/snd_internal.h"


void _SsSndDecrescendo(short seq, short sep) {
    SeqStruct *score = &g_SndSeqTable[seq][sep];
    SeqVolume left;
    SeqVolume right;

    score->fade_ticks_left--;

    if (score->fade_step > 0) {
        if ((score->fade_ticks_left % score->fade_step) == 0) {
            score->fade_steps_left--;
            if (score->fade_steps_left > 0) {
                SpuVmGetSeqVol((short)(seq | (sep << 8)), &left.output, &right.output);
                if (((left.value - score->fade_steps_left) > 0) &&
                    ((right.value - score->fade_steps_left) > 0) &&
                    (left.value != 1)) {
                    SpuVmSetSeqVol(seq | (sep << 8),
                                  left.value - 1,
                                  right.value - 1,
                                  0);
                } else {
                    SpuVmSetSeqVol(seq | (sep << 8), 1, 1, 0);
                }
            } else {
                g_SndSeqTable[seq][sep].flags &= ~0x20;
            }
            if ((score->fade_ticks_left == 0) || (score->fade_steps_left == 0)) {
                g_SndSeqTable[seq][sep].flags &= ~0x20;
            }
        }
    } else {
        score->fade_steps_left += score->fade_step;
        if (score->fade_steps_left > 0) {
            SpuVmGetSeqVol((short)(seq | (sep << 8)), &left.output, &right.output);
            if (((u_long)(long)score->fade_volume_range >=
                 ((score->fade_ticks_total - score->fade_ticks_left) * -score->fade_step)) &&
                (-score->fade_step < left.value)) {
                SpuVmSetSeqVol(seq | (sep << 8),
                              left.value + score->fade_step,
                              right.value + score->fade_step,
                              0);
            } else {
                SpuVmSetSeqVol(seq | (sep << 8), 1, 1, 0);
            }
        } else {
            g_SndSeqTable[seq][sep].flags &= ~0x20;
        }
        if ((score->fade_ticks_left == 0) || (score->fade_steps_left == 0)) {
            g_SndSeqTable[seq][sep].flags &= ~0x20;
        }
    }

    SpuVmGetSeqVol((short)(seq | (sep << 8)), &score->cur_vol_left, &score->cur_vol_right);
}
