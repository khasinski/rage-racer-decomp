#include "psyq/snd.h"

extern SeqStruct *D_801E79CC[];
extern s32 D_801E6C6C;

void func_80070A1C(s16 arg0, s16 arg1, u8 arg2);

void func_80070A1C(s16 arg0, s16 arg1, u8 arg2) {
    SeqStruct *pSeq = &D_801E79CC[arg0][arg1];

    if (arg2 != 0x2f) {
        if (arg2 == 0x51) {
            u8 *p;
            s32 value;

            p = pSeq->read_pos;
            pSeq->read_pos = p + 1;
            value = p[0] << 16;
            pSeq->read_pos = p + 2;
            value |= p[1] << 8;
            pSeq->read_pos = p + 3;
            value |= p[2];

            pSeq->tempo = 60000000 / value;

            if ((u32)(pSeq->tempo_multiplier * pSeq->tempo * 10) < (u32)(D_801E6C6C * 60)) {
                s16 q = (u32)(D_801E6C6C * 600) / (u32)(pSeq->tempo_multiplier * pSeq->tempo);
                pSeq->unk6E = q;
                pSeq->tick_period = q;
            } else {
                pSeq->unk6E = -1;
                pSeq->tick_period =
                    (u32)(pSeq->tempo_multiplier * pSeq->tempo * 10) / (u32)(D_801E6C6C * 60);
                if ((u32)(D_801E6C6C * 30) <
                    (u32)(pSeq->tempo_multiplier * pSeq->tempo * 10) % (u32)(D_801E6C6C * 60)) {
                    pSeq->tick_period++;
                }
            }

            pSeq->delta_value = SsSeqReadDeltaTime(arg0, arg1);
        }
    } else {
        s32 c = pSeq->unk48 + 1;
        pSeq->unk48 = c;

        if (pSeq->unk46 == 0) {
            pSeq->unk80 = 0;
            pSeq->unk27 = 0;
            pSeq->delta_value = 0;
            pSeq->read_pos = pSeq->next_sep_pos;
        } else if ((s16)c < pSeq->unk46) {
            pSeq->unk80 = 0;
            pSeq->unk27 = 0;
            pSeq->delta_value = 0;
            pSeq->read_pos = pSeq->next_sep_pos;
            pSeq->loop_pos = pSeq->next_sep_pos;
        } else {
            D_801E79CC[arg0][arg1].flags &= ~1;
            D_801E79CC[arg0][arg1].flags &= ~8;
            D_801E79CC[arg0][arg1].flags &= ~2;
            D_801E79CC[arg0][arg1].flags |= 0x200;
            D_801E79CC[arg0][arg1].flags |= 4;
            pSeq->unk2b = 0;
            pSeq->loop_pos = pSeq->next_sep_pos;
            if (pSeq->unk3C != 0xff) {
                pSeq->unk2b = 0;
                SsSeqRestartPlayback(pSeq->unk3C, pSeq->unk0);
                SpuVmSeqKeyOff((arg1 << 8) | arg0);
            }
            SpuVmSeqKeyOff((arg1 << 8) | arg0);
            pSeq->delta_value = pSeq->tick_period;
        }
    }
}
