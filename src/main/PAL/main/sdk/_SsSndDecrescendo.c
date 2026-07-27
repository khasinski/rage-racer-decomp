#include <sys/types.h>

#include "common.h"

typedef struct SeqStruct71568 {
    u_char pad00[0x3E];
    short unk3E;
    short unk40;
    short unk42;
    u_char pad44[0x34];
    short unk78;
    short unk7A;
    u_char pad7C[0x14];
    long flags;
    u_long unk94;
    u_long unk98;
    u_char pad9C[0x10];
} SeqStruct71568;

typedef union SeqVolume71568 {
    short output;
    u_short value;
} SeqVolume71568;

extern SeqStruct71568 *g_SndSeqTable[] asm("D_801E79CC");

long func_80076C58(short seq_sep, u_short left, u_short right, short update_voices);
long func_80076DCC(short seq_sep, short *left, short *right);

void _SsSndDecrescendo(short seq, short sep) asm("func_80071568");
void _SsSndDecrescendo(short seq, short sep) {
    SeqStruct71568 *score = &g_SndSeqTable[seq][sep];
    SeqVolume71568 left;
    SeqVolume71568 right;

    score->unk98--;

    if (score->unk42 > 0) {
        if ((score->unk98 % score->unk42) == 0) {
            score->unk40--;
            if (score->unk40 > 0) {
                func_80076DCC(seq | (sep << 8), &left.output, &right.output);
                if (((left.value - score->unk40) > 0) &&
                    ((right.value - score->unk40) > 0) &&
                    (left.value != 1)) {
                    func_80076C58(seq | (sep << 8),
                                  left.value - 1,
                                  right.value - 1,
                                  0);
                } else {
                    func_80076C58(seq | (sep << 8), 1, 1, 0);
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
            func_80076DCC(seq | (sep << 8), &left.output, &right.output);
            if (((u_long)(long)score->unk3E >=
                 ((score->unk94 - score->unk98) * -score->unk42)) &&
                (-score->unk42 < left.value)) {
                func_80076C58(seq | (sep << 8),
                              left.value + score->unk42,
                              right.value + score->unk42,
                              0);
            } else {
                func_80076C58(seq | (sep << 8), 1, 1, 0);
            }
        } else {
            g_SndSeqTable[seq][sep].flags &= ~0x20;
        }
        if ((score->unk98 == 0) || (score->unk40 == 0)) {
            g_SndSeqTable[seq][sep].flags &= ~0x20;
        }
    }

    func_80076DCC(seq | (sep << 8), &score->unk78, &score->unk7A);
}
