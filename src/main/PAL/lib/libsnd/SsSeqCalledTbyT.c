#include "psyq/snd.h"

#include "psyq/snd_internal.h"
void SsSeqCalledTbyT(void) {
    long i;
    long j;

    if (g_SndUpdateLock == 1) {
        return;
    }
    g_SndUpdateLock = 1;
    SsUtFlush();

    for (i = 0; i < g_SndSeqTableSMax; i++) {
        if (((1 << i) & g_SndSeqOpenMask) == 0) {
            continue;
        }
        for (j = 0; j < g_SndSeqTableTMax; j++) {
            if (g_SndSeqTable[i][j].flags % 2) {
                SsSeqAdvanceChannelTick((short)i, (short)j);
                if (g_SndSeqTable[i][j].flags & 0x10) {
                    _SsSndCrescendo((short)i, (short)j);
                }
                if (g_SndSeqTable[i][j].flags & 0x20) {
                    _SsSndDecrescendo((short)i, (short)j);
                }
                if (g_SndSeqTable[i][j].flags & 0x40) {
                    _SsSndTempo((short)i, (short)j);
                }
                if (g_SndSeqTable[i][j].flags & 0x80) {
                    _SsSndTempo((short)i, (short)j);
                }
            }
            if (g_SndSeqTable[i][j].flags & 0x2) {
                SsSeqPause((short)i, (short)j);
            }
            if (g_SndSeqTable[i][j].flags & 0x8) {
                SsSeqResume((short)i, (short)j);
            }
            if (g_SndSeqTable[i][j].flags & 0x4) {
                _SsSndStop(i, j);
                g_SndSeqTable[i][j].flags = 0;
            }
        }
    }
    g_SndUpdateLock = 0;
}


void _SsSndCrescendo(short seq, short sep) {
    SeqStruct *score = &g_SndSeqTable[seq][sep];
    SeqVolume left;
    SeqVolume right;
    long next_left;

    score->fade_ticks_left--;

    if (score->fade_step > 0) {
        if ((score->fade_ticks_left % (u_long)score->fade_step) == 0) {
            score->fade_steps_left--;
            if (score->fade_steps_left >= 0) {
                SpuVmGetSeqVol((short)(seq | (sep << 8)), &left.output, &right.output);
                next_left = left.value + 1;
                if ((left.value + score->fade_steps_left) >= next_left) {
                    SpuVmSetSeqVol(seq | (sep << 8),
                                  left.value + 1,
                                  right.value + 1,
                                  0);
                }
            } else {
                SpuVmSetSeqVol(seq | (sep << 8), 0x7F, 0x7F, 0);
                g_SndSeqTable[seq][sep].flags &= ~0x10;
            }
            if ((score->fade_ticks_left == 0) || (score->fade_steps_left == 0)) {
                g_SndSeqTable[seq][sep].flags &= ~0x10;
            }
        }
    } else if (score->fade_step < 0) {
        score->fade_steps_left += score->fade_step;
        if (score->fade_steps_left >= 0) {
            SpuVmGetSeqVol((short)(seq | (sep << 8)), &left.output, &right.output);
            if (((left.value - score->fade_step) >= 0x7F) &&
                ((right.value - score->fade_step) >= 0x7F)) {
                SpuVmSetSeqVol(seq | (sep << 8), 0x7F, 0x7F, 0);
            }
            if ((u_long)((score->fade_ticks_total - score->fade_ticks_left) * -score->fade_step) <
                (u_long)(long)score->fade_volume_range) {
                SpuVmSetSeqVol(seq | (sep << 8),
                              left.value - score->fade_step,
                              right.value - score->fade_step,
                              0);
            }
        } else {
            SpuVmSetSeqVol(seq | (sep << 8), 0x7F, 0x7F, 0);
            g_SndSeqTable[seq][sep].flags &= ~0x10;
        }
        if ((score->fade_ticks_left == 0) || (score->fade_steps_left == 0)) {
            g_SndSeqTable[seq][sep].flags &= ~0x10;
        }
    }

    SpuVmGetSeqVol((short)(seq | (sep << 8)), &score->cur_vol_left, &score->cur_vol_right);
}
