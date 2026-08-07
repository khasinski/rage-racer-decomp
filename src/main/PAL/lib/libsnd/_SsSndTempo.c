#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];
extern long g_SndTickResolution;

void _SsSndTempo(short seq, short sep) {
    SeqStruct *pSeq = &g_SndSeqTable[seq][sep];

    pSeq->tempo_countdown--;

    if (pSeq->tempo_step > 0) {
        if ((pSeq->tempo_countdown % (u_long)pSeq->tempo_step) == 0) {
            if (pSeq->tempo > pSeq->target_tempo) {
                pSeq->tempo--;
            } else if (pSeq->tempo < pSeq->target_tempo) {
                pSeq->tempo++;
            }

            pSeq->tick_period =
                (u_long)((pSeq->tempo_multiplier * pSeq->tempo) * 0xA) / (u_long)(g_SndTickResolution * 0x3C);
            if (pSeq->tick_period <= 0) {
                pSeq->tick_period = 1;
            }
            if ((pSeq->tempo_countdown == 0) || (pSeq->tempo == pSeq->target_tempo)) {
                g_SndSeqTable[seq][sep].flags &= ~0x40;
                g_SndSeqTable[seq][sep].flags &= ~0x80;
            }
        }
    } else {
        if (pSeq->tempo > pSeq->target_tempo) {
            pSeq->tempo = pSeq->tempo_step + pSeq->tempo;
            if (pSeq->tempo < pSeq->target_tempo) {
                pSeq->tempo = pSeq->target_tempo;
            }
        } else if (pSeq->tempo < pSeq->target_tempo) {
            pSeq->tempo = pSeq->tempo - pSeq->tempo_step;
            if (pSeq->target_tempo < pSeq->tempo) {
                pSeq->tempo = pSeq->target_tempo;
            }
        }

        pSeq->tick_period =
            (u_long)((pSeq->tempo_multiplier * pSeq->tempo) * 0xA) / (u_long)(g_SndTickResolution * 0x3C);
        if (pSeq->tick_period <= 0) {
            pSeq->tick_period = 1;
        }
        if ((pSeq->tempo_countdown == 0) || (pSeq->tempo == pSeq->target_tempo)) {
            g_SndSeqTable[seq][sep].flags &= ~0x40;
            g_SndSeqTable[seq][sep].flags &= ~0x80;
        }
    }
}
