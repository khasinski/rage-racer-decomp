#include "common.h"

typedef struct SeqStruct71568 {
    u8 pad00[0x3E];
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    u8 pad44[0x34];
    s16 unk78;
    s16 unk7A;
    u8 pad7C[0x14];
    s32 flags;
    u32 unk94;
    u32 unk98;
    u8 pad9C[0x10];
} SeqStruct71568;

typedef union SeqVolume71568 {
    s16 output;
    u16 value;
} SeqVolume71568;

extern SeqStruct71568 *D_801E79CC[];

s32 func_80076C58(s16 seq_sep, u16 left, u16 right, s16 update_voices);
s32 func_80076DCC(s16 seq_sep, s16 *left, s16 *right);

void func_80071568(s16 seq, s16 sep) {
    SeqStruct71568 *score = &D_801E79CC[seq][sep];
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
                D_801E79CC[seq][sep].flags &= ~0x20;
            }
            if ((score->unk98 == 0) || (score->unk40 == 0)) {
                D_801E79CC[seq][sep].flags &= ~0x20;
            }
        }
    } else {
        score->unk40 += score->unk42;
        if (score->unk40 > 0) {
            func_80076DCC(seq | (sep << 8), &left.output, &right.output);
            if (((u32)(s32)score->unk3E >=
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
            D_801E79CC[seq][sep].flags &= ~0x20;
        }
        if ((score->unk98 == 0) || (score->unk40 == 0)) {
            D_801E79CC[seq][sep].flags &= ~0x20;
        }
    }

    func_80076DCC(seq | (sep << 8), &score->unk78, &score->unk7A);
}
