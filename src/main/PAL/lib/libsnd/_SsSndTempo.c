#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];
extern long D_801E6C6C;

void _SsSndTempo(short arg0, short arg1) {
    SeqStruct *pSeq = &D_801E79CC[arg0][arg1];

    pSeq->tempo_countdown--;

    if (pSeq->tempo_step > 0) {
        if ((pSeq->tempo_countdown % (u_long)pSeq->tempo_step) == 0) {
            if (pSeq->tempo > pSeq->target_tempo) {
                pSeq->tempo--;
            } else if (pSeq->tempo < pSeq->target_tempo) {
                pSeq->tempo++;
            }

            pSeq->tick_period =
                (u_long)((pSeq->tempo_multiplier * pSeq->tempo) * 0xA) / (u_long)(D_801E6C6C * 0x3C);
            if (pSeq->tick_period <= 0) {
                pSeq->tick_period = 1;
            }
            if ((pSeq->tempo_countdown == 0) || (pSeq->tempo == pSeq->target_tempo)) {
                D_801E79CC[arg0][arg1].flags &= ~0x40;
                D_801E79CC[arg0][arg1].flags &= ~0x80;
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
            (u_long)((pSeq->tempo_multiplier * pSeq->tempo) * 0xA) / (u_long)(D_801E6C6C * 0x3C);
        if (pSeq->tick_period <= 0) {
            pSeq->tick_period = 1;
        }
        if ((pSeq->tempo_countdown == 0) || (pSeq->tempo == pSeq->target_tempo)) {
            D_801E79CC[arg0][arg1].flags &= ~0x40;
            D_801E79CC[arg0][arg1].flags &= ~0x80;
        }
    }
}
