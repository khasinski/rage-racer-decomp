#include "psyq/snd.h"

void _SsSndCrescendo(s16 seq, s16 sep) asm("func_8007128C");

extern volatile s32 D_801E40AC;
extern SeqStruct *D_801E79CC[];
extern s32 D_801E4B80;
extern s16 D_801E826C;
extern s16 D_801E8270;
void _SsSndDecrescendo(s16 seq, s16 sep) asm("func_80071568");
void func_80072734(s32 seq, s32 sep);

void SsSeqCalledTbyT(void) asm("func_80071018");
void SsSeqCalledTbyT(void) {
    s32 i;
    s32 j;

    if (D_801E40AC == 1) {
        return;
    }
    D_801E40AC = 1;
    SsUtFlush();

    for (i = 0; i < D_801E826C; i++) {
        if (((1 << i) & D_801E4B80) == 0) {
            continue;
        }
        for (j = 0; j < D_801E8270; j++) {
            if (D_801E79CC[i][j].flags & 0x1) {
                SsSeqAdvanceChannelTick((s16)i, (s16)j);
                if (D_801E79CC[i][j].flags & 0x10) {
                    _SsSndCrescendo((s16)i, (s16)j);
                }
                if (D_801E79CC[i][j].flags & 0x20) {
                    _SsSndDecrescendo((s16)i, (s16)j);
                }
                if (D_801E79CC[i][j].flags & 0x40) {
                    _SsSndTempo((s16)i, (s16)j);
                }
                if (D_801E79CC[i][j].flags & 0x80) {
                    _SsSndTempo((s16)i, (s16)j);
                }
            }
            if (D_801E79CC[i][j].flags & 0x2) {
                SsSeqPause((s16)i, (s16)j);
            }
            if (D_801E79CC[i][j].flags & 0x8) {
                SsSeqResume((s16)i, (s16)j);
            }
            if (D_801E79CC[i][j].flags & 0x4) {
                func_80072734(i, j);
                D_801E79CC[i][j].flags = 0;
            }
        }
    }
    D_801E40AC = 0;
}

typedef union SeqVolume7128C {
    s16 output;
    u16 value;
} SeqVolume7128C;

s32 func_80076C58(s16 seq_sep, u16 left, u16 right, s16 update_voices);
s32 func_80076DCC(s16 seq_sep, s16 *left, s16 *right);

void _SsSndCrescendo(s16 seq, s16 sep) asm("func_8007128C");
void _SsSndCrescendo(s16 seq, s16 sep) {
    SeqStruct *score = &D_801E79CC[seq][sep];
    SeqVolume7128C left;
    SeqVolume7128C right;
    s32 next_left;

    score->unk98--;

    if (score->unk42 > 0) {
        if ((score->unk98 % (u32)score->unk42) == 0) {
            score->unk40--;
            if (score->unk40 >= 0) {
                func_80076DCC(seq | (sep << 8), &left.output, &right.output);
                next_left = left.value + 1;
                if ((left.value + score->unk40) >= next_left) {
                    func_80076C58(seq | (sep << 8),
                                  left.value + 1,
                                  right.value + 1,
                                  0);
                }
            } else {
                func_80076C58(seq | (sep << 8), 0x7F, 0x7F, 0);
                D_801E79CC[seq][sep].flags &= ~0x10;
            }
            if ((score->unk98 == 0) || (score->unk40 == 0)) {
                D_801E79CC[seq][sep].flags &= ~0x10;
            }
        }
    } else if (score->unk42 < 0) {
        score->unk40 += score->unk42;
        if (score->unk40 >= 0) {
            func_80076DCC(seq | (sep << 8), &left.output, &right.output);
            if (((left.value - score->unk42) >= 0x7F) &&
                ((right.value - score->unk42) >= 0x7F)) {
                func_80076C58(seq | (sep << 8), 0x7F, 0x7F, 0);
            }
            if ((u32)((score->unk94 - score->unk98) * -score->unk42) <
                (u32)(s32)score->unk3E) {
                func_80076C58(seq | (sep << 8),
                              left.value - score->unk42,
                              right.value - score->unk42,
                              0);
            }
        } else {
            func_80076C58(seq | (sep << 8), 0x7F, 0x7F, 0);
            D_801E79CC[seq][sep].flags &= ~0x10;
        }
        if ((score->unk98 == 0) || (score->unk40 == 0)) {
            D_801E79CC[seq][sep].flags &= ~0x10;
        }
    }

    func_80076DCC(seq | (sep << 8), &score->unk78, &score->unk7A);
}
