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
} SpuVoice;

extern s32 D_801E40AC;
extern u16 D_801E4BEA;
extern SpuVoice D_8009E0B8[];
extern s16 D_8009E0C4[];
extern s16 D_8009E0CA[];
extern s16 D_8009E0CC[];
extern s16 D_8009E0CE[];
extern u16 D_801F2A08;
extern u16 D_801F2A0C;
extern u16 D_8009E670;
extern u16 D_8009E674;
extern u16 *D_8009A588;

s32 func_80077A88(s32 voice, s32 vab_id, s32 program, s32 tone, s32 note) {
    s16 original_voice;
    s32 index;
    u8 new_var;
    u16 bits_upper;
    u16 bits_lower;
    u16 current_voice;

    if (D_801E40AC == 1) {
        return -1;
    }
    original_voice = voice;
    D_801E40AC = 1;

    if ((u16)original_voice < 24) {
        index = (s16)voice;
        voice = ((((index * 2) + index) << 2) + index) << 2;
        if (*(s16 *)((u8 *)D_8009E0CE + voice) == (s16)vab_id &&
            *(s16 *)((u8 *)D_8009E0CA + voice) == (s16)program &&
            *(s16 *)((u8 *)D_8009E0CC + voice) == (s16)tone &&
            *(s16 *)((u8 *)D_8009E0C4 + voice) == (s16)note) {
            if (*(s16 *)((u8 *)D_8009E0B8 + voice) == 0xFF) {
                new_var = original_voice;
                D_8009E0B8[new_var].active = 0;
                D_8009E0B8[new_var].pitch = 0;
                D_8009A588[202] = 0;
                D_8009A588[203] = 0;
            } else {
                *(s16 *)&D_801E4BEA = original_voice;
                current_voice = D_801E4BEA;
                if (current_voice < 16) {
                    bits_lower = 1 << current_voice;
                    bits_upper = 0;
                } else {
                    bits_lower = 0;
                    bits_upper = 1 << (current_voice - 16);
                }
                D_8009E0B8[current_voice].active = 0;
                D_8009E0B8[current_voice].pitch = 0;
                D_8009E0B8[current_voice].vag = 0;
                D_801F2A08 = bits_lower | D_801F2A08;
                D_801F2A0C = bits_upper | D_801F2A0C;
                D_8009E670 &= ~D_801F2A08;
                D_8009E674 &= ~D_801F2A0C;
            }
            D_801E40AC = 0;
            return 0;
        }
    }
    D_801E40AC = 0;
    return -1;
}
