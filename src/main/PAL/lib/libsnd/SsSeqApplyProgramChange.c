#include "psyq/snd.h"

extern SeqStruct *g_SndSeqTable[];

void SsSeqApplyProgramChange(long seq, long sep) {
    SeqStruct *state = &g_SndSeqTable[(short)seq][(short)sep];
    u_char *read_pos = state->read_pos;

    state->read_pos++;
    SpuVmApplyPitchBendByTone(((short)sep << 8) | (short)seq, state->unk4c, *((u_char *)((long)state->channel + (long)state + 0x2C)), *read_pos);
    state->delta_value = SsSeqReadDeltaTime((short)seq, (short)sep);
}

extern SeqStruct *g_SndSeqTable[];
extern long g_SndTickResolution;

void SsSeqSetChannelProgram(short arg0, short arg1, u_char arg2);

void SsSeqSetChannelProgram(short arg0, short arg1, u_char arg2) {
    SeqStruct *pSeq = &g_SndSeqTable[arg0][arg1];

    if (arg2 != 0x2f) {
        if (arg2 == 0x51) {
            u_char *p;
            long value;

            p = pSeq->read_pos;
            pSeq->read_pos = p + 1;
            value = p[0] << 16;
            pSeq->read_pos = p + 2;
            value |= p[1] << 8;
            pSeq->read_pos = p + 3;
            value |= p[2];

            pSeq->tempo = 60000000 / value;

            if ((u_long)(pSeq->tempo_multiplier * pSeq->tempo * 10) < (u_long)(g_SndTickResolution * 60)) {
                short q = (u_long)(g_SndTickResolution * 600) / (u_long)(pSeq->tempo_multiplier * pSeq->tempo);
                pSeq->unk6E = q;
                pSeq->tick_period = q;
            } else {
                pSeq->unk6E = -1;
                pSeq->tick_period =
                    (u_long)(pSeq->tempo_multiplier * pSeq->tempo * 10) / (u_long)(g_SndTickResolution * 60);
                if ((u_long)(g_SndTickResolution * 30) <
                    (u_long)(pSeq->tempo_multiplier * pSeq->tempo * 10) % (u_long)(g_SndTickResolution * 60)) {
                    pSeq->tick_period++;
                }
            }

            pSeq->delta_value = SsSeqReadDeltaTime(arg0, arg1);
        }
    } else {
        long c = pSeq->unk48 + 1;
        pSeq->unk48 = c;

        if (pSeq->unk46 == 0) {
            pSeq->unk80 = 0;
            pSeq->unk27 = 0;
            pSeq->delta_value = 0;
            pSeq->read_pos = pSeq->next_sep_pos;
        } else if ((short)c < pSeq->unk46) {
            pSeq->unk80 = 0;
            pSeq->unk27 = 0;
            pSeq->delta_value = 0;
            pSeq->read_pos = pSeq->next_sep_pos;
            pSeq->loop_pos = pSeq->next_sep_pos;
        } else {
            g_SndSeqTable[arg0][arg1].flags &= ~1;
            g_SndSeqTable[arg0][arg1].flags &= ~8;
            g_SndSeqTable[arg0][arg1].flags &= ~2;
            g_SndSeqTable[arg0][arg1].flags |= 0x200;
            g_SndSeqTable[arg0][arg1].flags |= 4;
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

extern SeqStruct *g_SndSeqTable[];

long SsSeqReadDeltaTime(long arg0, long arg1) {
    SeqStruct *state;
    u_char *stream;
    long value;
    long byte;
    long ret;

    state = &g_SndSeqTable[(short)arg0][(short)arg1];

    stream = state->read_pos;
    state->read_pos = stream + 1;
    value = stream[0];

    if (value == 0) {
        return 0;
    }

    if (value & 0x80) {
        value &= 0x7F;
        do {
            stream = state->read_pos;
            state->read_pos = stream + 1;
            byte = stream[0];
            value = (value << 7) + (byte & 0x7F);
        } while (byte & 0x80);
    }

    ret = value * 10;
    state->unk80 += ret;
    return ret;
}

void SsUnpackAdsr(u_long arg0, u_long arg1, u_short *out) {
    out[5] = arg0 & 0x8000;
    out[6] = arg1 & 0x8000;
    out[8] = arg1 & 0x4000;
    out[7] = arg1 & 0x20;
    out[0] = ((arg0 & 0xFFFF) >> 8) & 0x7F;
    out[1] = ((arg0 & 0xFFFF) >> 4) & 0xF;
    out[2] = arg0 & 0xF;
    out[3] = (arg1 >> 6) & 0x7F;
    out[4] = arg1 & 0x1F;
}

void SsPackAdsr(u_short *in, u_short *out0, u_short *out1) {
    u_long mask = 0xFFFF8000;
    long has8000 = ((short *)in)[6];
    register long has4000 asm("$3") = ((short *)in)[8];
    u_long pred;
    u_long word1High;
    long hasWord0High;
    register u_long word1 asm("$8");
    u_long word0High;
    u_long word0;
    register u_long tmp asm("$2");
    register u_long tmp2 asm("$3");

    pred = has8000 != 0;
    pred = -pred;
    word1High = pred & mask;
    hasWord0High = ((short *)in)[5];
    word1 = word1High;
    pred = hasWord0High != 0;
    pred = -pred;
    word0High = pred & mask;

    if (has4000 != 0) {
        word1 = word1High | 0x4000;
    }

    word0 = word0High | ((in[0] << 8) & 0x7F00);
    tmp = (in[1] << 4) & 0xF0;
    word0 = word0 | tmp;
    tmp2 = in[2] & 0xF;
    word0 = word0 | tmp2;

    tmp = in[3];
    tmp2 = in[4];
    *out0 = word0;

    tmp = (tmp << 6) & 0x1FC0;
    tmp = word1 | tmp;
    tmp2 = tmp2 & 0x1F;
    word1 = tmp | tmp2;
    *out1 = word1;
}

extern SeqStruct *g_SndSeqTable[];

void SsSeqRestartPlayback(short arg0, short arg1) {
    SeqStruct *state;

    state = &g_SndSeqTable[arg0][arg1];
    state->unk46 = 1;
    state->unk48 = 0;

    g_SndSeqTable[arg0][arg1].flags &= ~0x100;
    g_SndSeqTable[arg0][arg1].flags &= ~0x8;
    g_SndSeqTable[arg0][arg1].flags &= ~0x2;
    g_SndSeqTable[arg0][arg1].flags &= ~0x4;
    g_SndSeqTable[arg0][arg1].flags &= ~0x200;

    state->read_pos = state->next_sep_pos;
    state->unk2b = 1;
    g_SndSeqTable[arg0][arg1].flags |= 1;
}
