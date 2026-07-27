#include <sys/types.h>

#include "common.h"

typedef struct {
    short vag;
    short age;
    short pitch;
    u_short env;
    short base_volume;
    signed char pan;
    signed char unkB;
    short note;
    short seq_sep;
    short program_index;
    short program;
    short tone;
    short vab_id;
    short priority;
    u_char pad1A;
    u_char active;
    short auto_volume;
    short unk1E;
    short unk20;
    short unk22;
    short start_volume;
    short end_volume;
    short auto_pan;
    short unk2A;
    short unk2C;
    short unk2E;
    short start_pan;
    short end_pan;
} SpuVoice;

extern long D_801E40AC;
extern u_short D_801E4BEA;
extern SpuVoice D_8009E0B8[];
extern short D_8009E0C4[];
extern short D_8009E0CA[];
extern short D_8009E0CC[];
extern short D_8009E0CE[];
extern u_short D_801F2A08;
extern u_short D_801F2A0C;
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_short *D_8009A588;

long SsUtKeyOff(long voice, long vab_id, long program, long tone, long note) asm("func_80077A88");
long SsUtKeyOff(long voice, long vab_id, long program, long tone, long note) {
    short original_voice;
    long index;
    u_char new_var;
    u_short bits_upper;
    u_short bits_lower;
    u_short current_voice;

    if (D_801E40AC == 1) {
        return -1;
    }
    original_voice = voice;
    D_801E40AC = 1;

    if ((u_short)original_voice < 24) {
        index = (short)voice;
        voice = ((((index * 2) + index) << 2) + index) << 2;
        if (*(short *)((u_char *)D_8009E0CE + voice) == (short)vab_id &&
            *(short *)((u_char *)D_8009E0CA + voice) == (short)program &&
            *(short *)((u_char *)D_8009E0CC + voice) == (short)tone &&
            *(short *)((u_char *)D_8009E0C4 + voice) == (short)note) {
            if (*(short *)((u_char *)D_8009E0B8 + voice) == 0xFF) {
                new_var = original_voice;
                D_8009E0B8[new_var].active = 0;
                D_8009E0B8[new_var].pitch = 0;
                D_8009A588[202] = 0;
                D_8009A588[203] = 0;
            } else {
                *(short *)&D_801E4BEA = original_voice;
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
