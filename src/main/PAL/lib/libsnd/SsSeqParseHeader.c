#include "common.h"
#include <stdio.h>
#include "psyq/snd.h"

#include "psyq/snd_internal.h"


long SsSeqParseHeader(long slot, long vabId, long data) {
    SeqStruct *s;
    u_char *seqBytes;
    u_char *p;
    long i;
    long hi, lo, b0, b1, b2;
    long v24;
    long q;
    long ret;
    u_long D;
    register long prod asm("$6");
    register long vabReg asm("$4");
    register long slotReg;

    seqBytes = (u_char *)data;
    slotReg = slot;
    vabReg = slot;
    __asm__("" : "=r"(vabReg) : "0"(vabReg));
    s = (SeqStruct *)g_SndSeqTable[(short)vabReg];

    s->unk4c = vabId;
    s->tempo_multiplier = 0;
    s->unk13 = 0;
    s->play_mode = 0;
    s->unk29 = 0;
    s->unk15 = 0;
    s->unk16 = 0;
    s->unk2a = 0;
    s->channel = 0;
    s->base_delta_value = 0;
    s->unk80 = 0;
    s->base_unk84 = 0;
    s->unk72 = 0;
    s->unk48 = 0;
    s->unk2b = 0;
    s->delta_value = 0;
    s->unk27 = 0;
    s->unk28 = 0;
    s->unk10 = 0;
    s->unk11 = 0;
    s->padA8 = 0x7f;
    s->padAA = 0;
    for (i = 0; i < 16; i++) {
        s->programs[i] = i;
        s->panpot[i] = 0x40;
        s->vol[i] = 0x7f;
    }
    s->unk6E = 1;
    s->read_pos = seqBytes;

    if (!(seqBytes[0] != 'S' && seqBytes[0] != 'p')) {
    s->read_pos = seqBytes + 8;
    if (seqBytes[7] == 1) {
    } else {
    printf(g_MsgSeqNotSeqData);
    return -1;
    }
    } else {
    printf(g_MsgSeqOldFormat);
    return 0;
    }


    p = s->read_pos; s->read_pos = p + 1; hi = *p;
    p = s->read_pos; s->read_pos = p + 1; lo = *p;
    s->tempo_multiplier = lo | (hi << 8);

    p = s->read_pos;
    s->read_pos = p + 1; b0 = p[0];
    s->read_pos = p + 2; b1 = p[1];
    s->read_pos = p + 3; b2 = p[2];
    v24 = (b0 << 16) | (b1 << 8) | b2;

    q = 60000000 / v24;
    s->base_unk84 = v24;
    if ((long)((u_long)v24 >> 1) < 60000000 % v24)
        s->base_unk84 = q + 1;
    else
        s->base_unk84 = q;

    s->tempo = s->base_unk84;
    s->read_pos = s->read_pos + 2;
    ret = SsSeqReadDeltaTime((short)slotReg, 0);

    s->next_sep_pos = s->read_pos;
    D = g_SndTickResolution;
    s->base_delta_value = ret;
    s->delta_value = ret;
    s->loop_pos = s->read_pos;

    prod = s->tempo_multiplier * s->base_unk84;
    if ((u_long)(prod * 10) < D * 60) {
        long qi = (D * 600) / (u_long)prod;
        s->unk6E = qi;
        s->tick_period = qi;
    } else {
        long qe;
        __asm__ volatile("" ::: "memory");
        qe = (u_long)(s->tempo_multiplier * s->base_unk84 * 10) / (D * 60);
        s->unk6E = -1;
        s->tick_period = qe;
        if (D * 30 < (u_long)(s->tempo_multiplier * s->base_unk84 * 10) % (D * 60))
            s->tick_period = qe + 1;
    }
    s->unk72 = s->tick_period;
    return 0;
}
