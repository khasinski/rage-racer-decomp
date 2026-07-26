#include "common.h"

typedef struct {
    s16 vag;
    s16 age;
    s16 pitch;
    u16 env;
    s16 base_volume;
    s8 pan;
    s8 unkB;
    s16 note;
    s16 seq_sep;
    s16 program_index;
    s16 program;
    s16 tone;
    s16 vab_id;
    s16 priority;
    u8 pad1A;
    u8 active;
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
} SpuVoice76940;

typedef struct {
    u8 pad[0x1A];
    s16 voice;
} SvmCurrent76940;

extern SpuVoice76940 D_8009E0B8[];
extern u16 D_8009E670;
extern u16 D_8009E674;
extern u16 *D_8009A588;
extern u8 D_801E42F8;
extern SvmCurrent76940 D_801E4BD0;
extern u16 D_801F2A08;
extern u16 D_801F2A0C;

static inline u16 get_selected_voice(void) {
    return D_801E4BD0.voice;
}

s32 SpuVmSeKeyOff(s16 seq_sep, s16 vab_id, s16 program, u16 note) asm("func_80076940");
s32 SpuVmSeKeyOff(s16 seq_sep, s16 vab_id, s16 program, u16 note) {
    u16 bits_upper;
    u16 bits_lower;
    u8 voice;
    s32 count;
    s32 selected_voice;
    u32 selected_index;

    count = 0;
    for (voice = 0; voice < D_801E42F8; voice++) {
        if ((D_8009E0B8[voice].note == note) &&
            (D_8009E0B8[voice].program == program) &&
            (D_8009E0B8[voice].seq_sep == seq_sep) &&
            (D_8009E0B8[voice].vab_id == vab_id)) {
            if (D_8009E0B8[voice].vag == 0xFF) {
                D_8009E0B8[voice].active = 0;
                D_8009E0B8[voice].pitch = 0;
                D_8009A588[202] = 0;
                D_8009A588[203] = 0;
            } else {
                D_801E4BD0.voice = voice;
                selected_voice = get_selected_voice();
                selected_index = selected_voice & 0xFFFF;
                if (selected_index < 0x10) {
                    bits_lower = 1 << selected_index;
                    bits_upper = 0;
                } else {
                    bits_lower = 0;
                    bits_upper = 1 << (selected_index - 0x10);
                }

                D_8009E0B8[(u16)selected_voice].active = 0;
                D_8009E0B8[(u16)selected_voice].pitch = 0;
                D_8009E0B8[(u16)selected_voice].vag = 0;

                D_801F2A08 = bits_lower | D_801F2A08;
                D_801F2A0C = bits_upper | D_801F2A0C;

                D_8009E670 &= ~D_801F2A08;
                D_8009E674 &= ~D_801F2A0C;
            }
            count++;
        }
    }
    return count;
}
