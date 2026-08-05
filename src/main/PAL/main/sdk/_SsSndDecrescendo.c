#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

typedef union SeqVolume71568 {
    short output;
    u_short value;
} SeqVolume71568;

extern SeqStruct *g_SndSeqTable[];

long SpuVmSetSeqVol(short seq_sep, u_short left, u_short right, short update_voices);
long SpuVmGetSeqVol(short seq_sep, short *left, short *right);

void _SsSndDecrescendo(short seq, short sep) {
    SeqStruct *score = &g_SndSeqTable[seq][sep];
    SeqVolume71568 left;
    SeqVolume71568 right;

    score->unk98--;

    if (score->unk42 > 0) {
        if ((score->unk98 % score->unk42) == 0) {
            score->unk40--;
            if (score->unk40 > 0) {
                SpuVmGetSeqVol(seq | (sep << 8), &left.output, &right.output);
                if (((left.value - score->unk40) > 0) &&
                    ((right.value - score->unk40) > 0) &&
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
            if ((score->unk98 == 0) || (score->unk40 == 0)) {
                g_SndSeqTable[seq][sep].flags &= ~0x20;
            }
        }
    } else {
        score->unk40 += score->unk42;
        if (score->unk40 > 0) {
            SpuVmGetSeqVol(seq | (sep << 8), &left.output, &right.output);
            if (((u_long)(long)score->unk3E >=
                 ((score->unk94 - score->unk98) * -score->unk42)) &&
                (-score->unk42 < left.value)) {
                SpuVmSetSeqVol(seq | (sep << 8),
                              left.value + score->unk42,
                              right.value + score->unk42,
                              0);
            } else {
                SpuVmSetSeqVol(seq | (sep << 8), 1, 1, 0);
            }
        } else {
            g_SndSeqTable[seq][sep].flags &= ~0x20;
        }
        if ((score->unk98 == 0) || (score->unk40 == 0)) {
            g_SndSeqTable[seq][sep].flags &= ~0x20;
        }
    }

    SpuVmGetSeqVol(seq | (sep << 8), &score->unk78, &score->unk7A);
}
