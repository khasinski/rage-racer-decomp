#include "common.h"

typedef struct {
    u8 pad00[0x74];
    u16 left_volume;
    u16 right_volume;
    u8 pad78[0x34];
} SeqState74348;

typedef struct {
    s16 vag;
    s16 age;
    s16 pitch;
    u16 unk6;
    s16 base_volume;
    s8 unkA;
    s8 unkB;
    s16 note;
    s16 seq_sep;
    s16 program_index;
    s16 program;
    s16 tone;
    s16 vab_id;
    s16 priority;
    u8 pad1A;
    u8 status;
    s16 auto_volume;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 start_volume;
    s16 end_volume;
    s16 auto_pan;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 start_pan;
    s16 end_pan;
} VoiceState74348;

typedef struct {
    u8 tones;
    u8 vab_id;
    u8 note;
    u8 fine;
    u8 volume;
    u8 pan;
    u8 program;
    u8 fake_program;
    u8 unk8;
    u8 unk9;
    u8 master_volume;
    u8 master_pan;
    u8 tone;
    u8 tone_volume;
    u8 tone_pan;
    u8 priority;
    u8 center;
    u8 shift;
    u8 min;
    u8 max;
    u8 mode;
    u8 pad15;
    u16 seq_sep;
    s16 vag;
    u16 voice;
} SvmCurrent74348;

extern SeqState74348 *D_801E79CC[];
extern VoiceState74348 D_8009E0B8[];
extern SvmCurrent74348 D_801E4BD0;
extern u16 D_8009DF20[];
extern u8 D_8009E0A0[];
extern u8 D_801E42F8;
extern s16 D_801E3FB0;
extern volatile u16 *D_8009A588;
extern volatile u16 D_801F2A08;
extern volatile u16 D_801F2A0C;
extern u16 D_8009E670;
extern u16 D_8009E674;
extern u16 D_8009E680;
extern u16 D_8009E684;

void func_80074348(u8 voice) {
    SeqState74348 *score =
        &D_801E79CC[D_801E4BD0.seq_sep & 0xFF]
                    [(D_801E4BD0.seq_sep & 0xFF00) >> 8];
    s16 current_voice;
    u16 bits_upper;
    u16 bits_lower;
    u32 left_temp;
    u32 right_temp;
    u32 left;
    u32 right;
    u16 control;
    u32 pan;

    control = D_8009A588[0x1AA / 2];

    left_temp = score->left_volume * 0x81;
    right_temp = score->right_volume * 0x81;

    left_temp = (left_temp * D_801E4BD0.master_volume) / 0x7F;
    right_temp = (right_temp * D_801E4BD0.master_volume) / 0x7F;

    left_temp = (left_temp * D_801E4BD0.tone_volume) / 0x7F;
    right_temp = (right_temp * D_801E4BD0.tone_volume) / 0x7F;

    pan = D_801E4BD0.tone_pan;
    if (pan < 0x40) {
        left = left_temp;
        right = (right_temp * pan) / 0x3F;
    } else {
        left = (left_temp * (0x7F - pan)) / 0x3F;
        right = right_temp;
    }

    pan = D_801E4BD0.master_pan;
    if (pan < 0x40) {
        right = (right * pan) / 0x3F;
    } else {
        left = (left * (0x7F - pan)) / 0x3F;
    }

    pan = D_801E4BD0.pan;
    if (pan < 0x40) {
        right = (pan * right) / 0x3F;
    } else {
        left = (left * (0x7F - pan)) / 0x3F;
    }

    if (D_801E3FB0 == 1) {
        if (left < right) {
            left = right;
        } else {
            right = left;
        }
    }

    control &= ~0x3F00;
    control |= ((D_801E4BD0.note - D_801E4BD0.center) & 0x3F) << 8;
    D_8009A588[0x1AA / 2] = control;

    D_8009DF20[voice * 8] = left;
    D_8009DF20[voice * 8 + 1] = right;
    D_8009E0A0[voice] |= 3;

    if (voice < 0x10) {
        bits_lower = 1 << voice;
        bits_upper = 0;
    } else {
        bits_lower = 0;
        bits_upper = 1 << (voice - 0x10);
    }

    D_8009E0B8[voice].pitch = 0xA;
    for (current_voice = 0; current_voice < D_801E42F8; current_voice++) {
        D_8009E0B8[current_voice].status &= 1;
    }
    D_8009E0B8[voice].status = 2;

    D_8009E670 |= bits_lower;
    D_8009E674 |= bits_upper;
    D_801F2A08 &= ~D_8009E670;
    D_801F2A0C &= ~D_8009E674;

    if (D_801E4BD0.mode & 4) {
        D_8009E680 |= bits_lower;
        D_8009E684 |= bits_upper;
    } else {
        D_8009E680 &= ~bits_lower;
        D_8009E684 &= ~bits_upper;
    }

    D_8009A588[0x194 / 2] = bits_lower;
    D_8009A588[0x196 / 2] = bits_upper;
}
