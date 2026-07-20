#include "common.h"
#include "psyq/snd.h"

extern u8 *D_801E79CC[];
extern u32 D_801E6C6C;
extern char D_80013BD4[];
extern char D_80013BEC[];

void func_8001674C(char *fmt);
s32 func_80070D70(s16 arg0, s16 arg1);

s32 func_8006ECDC(s32 arg0, s32 arg1, s32 arg2) {
    SeqStruct *s;
    register u8 *seq asm("$7");
    u8 *p;
    s32 i;
    s32 hi, lo, b0, b1, b2;
    s32 v24;
    s32 q;
    s32 ret;
    u32 D;
    register s32 prod asm("$6");
    register s32 vab asm("$4");
    register s32 slot asm("$9");

    seq = (u8 *)arg2;
    __asm__("" : "=r"(seq) : "0"(seq));
    slot = arg0;
    __asm__("" : "=r"(slot) : "0"(slot));
    vab = arg0;
    __asm__("" : "=r"(vab) : "0"(vab));
    s = (SeqStruct *)D_801E79CC[(s16)vab];

    s->unk4c = arg1;
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
    s->read_pos = seq;

    if (seq[0] != 'S' && seq[0] != 'p') {
        goto magic_err;
    }
    s->read_pos = seq + 8;
    if (seq[7] == 1) {
        goto ok;
    }
    func_8001674C(D_80013BD4);
    return -1;
magic_err:
    func_8001674C(D_80013BEC);
    return 0;
ok:

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
    if ((s32)((u32)v24 >> 1) < 60000000 % v24)
        s->base_unk84 = q + 1;
    else
        s->base_unk84 = q;

    s->tempo = s->base_unk84;
    s->read_pos = s->read_pos + 2;
    ret = func_80070D70((s16)slot, 0);

    s->next_sep_pos = s->read_pos;
    D = D_801E6C6C;
    s->base_delta_value = ret;
    s->delta_value = ret;
    s->loop_pos = s->read_pos;

    prod = s->tempo_multiplier * s->base_unk84;
    if ((u32)(prod * 10) < D * 60) {
        s32 qi = (D * 600) / (u32)prod;
        s->unk6E = qi;
        s->tick_period = qi;
    } else {
        s32 qe;
        __asm__ volatile("" ::: "memory");
        qe = (u32)(s->tempo_multiplier * s->base_unk84 * 10) / (D * 60);
        s->unk6E = -1;
        s->tick_period = qe;
        if (D * 30 < (u32)(s->tempo_multiplier * s->base_unk84 * 10) % (D * 60))
            s->tick_period = qe + 1;
    }
    s->unk72 = s->tick_period;
    return 0;
}
