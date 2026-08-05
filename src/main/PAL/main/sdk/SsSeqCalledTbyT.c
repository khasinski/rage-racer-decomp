#include "psyq/snd.h"

void _SsSndCrescendo(short seq, short sep) asm("func_8007128C");

extern volatile long g_SndUpdateLock asm("D_801E40AC");
extern SeqStruct *g_SndSeqTable[] asm("D_801E79CC");
extern long g_SndSeqOpenMask asm("D_801E4B80");
extern short g_SndSeqTableSMax asm("D_801E826C");
extern short g_SndSeqTableTMax asm("D_801E8270");
void _SsSndDecrescendo(short seq, short sep) asm("func_80071568");
void func_80072734(long seq, long sep);

void SsSeqCalledTbyT(void) asm("func_80071018");
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
                func_80072734(i, j);
                g_SndSeqTable[i][j].flags = 0;
            }
        }
    }
    g_SndUpdateLock = 0;
}

typedef union SeqVolume7128C {
    short output;
    u_short value;
} SeqVolume7128C;

long SpuVmSetSeqVol(short seq_sep, u_short left, u_short right, short update_voices) asm("func_80076C58");
long SpuVmGetSeqVol(short seq_sep, short *left, short *right) asm("func_80076DCC");

void _SsSndCrescendo(short seq, short sep) asm("func_8007128C");
void _SsSndCrescendo(short seq, short sep) {
    SeqStruct *score = &g_SndSeqTable[seq][sep];
    SeqVolume7128C left;
    SeqVolume7128C right;
    long next_left;

    score->unk98--;

    if (score->unk42 > 0) {
        if ((score->unk98 % (u_long)score->unk42) == 0) {
            score->unk40--;
            if (score->unk40 >= 0) {
                SpuVmGetSeqVol(seq | (sep << 8), &left.output, &right.output);
                next_left = left.value + 1;
                if ((left.value + score->unk40) >= next_left) {
                    SpuVmSetSeqVol(seq | (sep << 8),
                                  left.value + 1,
                                  right.value + 1,
                                  0);
                }
            } else {
                SpuVmSetSeqVol(seq | (sep << 8), 0x7F, 0x7F, 0);
                g_SndSeqTable[seq][sep].flags &= ~0x10;
            }
            if ((score->unk98 == 0) || (score->unk40 == 0)) {
                g_SndSeqTable[seq][sep].flags &= ~0x10;
            }
        }
    } else if (score->unk42 < 0) {
        score->unk40 += score->unk42;
        if (score->unk40 >= 0) {
            SpuVmGetSeqVol(seq | (sep << 8), &left.output, &right.output);
            if (((left.value - score->unk42) >= 0x7F) &&
                ((right.value - score->unk42) >= 0x7F)) {
                SpuVmSetSeqVol(seq | (sep << 8), 0x7F, 0x7F, 0);
            }
            if ((u_long)((score->unk94 - score->unk98) * -score->unk42) <
                (u_long)(long)score->unk3E) {
                SpuVmSetSeqVol(seq | (sep << 8),
                              left.value - score->unk42,
                              right.value - score->unk42,
                              0);
            }
        } else {
            SpuVmSetSeqVol(seq | (sep << 8), 0x7F, 0x7F, 0);
            g_SndSeqTable[seq][sep].flags &= ~0x10;
        }
        if ((score->unk98 == 0) || (score->unk40 == 0)) {
            g_SndSeqTable[seq][sep].flags &= ~0x10;
        }
    }

    SpuVmGetSeqVol(seq | (sep << 8), &score->unk78, &score->unk7A);
}
