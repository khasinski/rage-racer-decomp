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
} SpuVoice76940;

typedef struct {
    u_char pad[0x1A];
    short voice;
} SvmCurrent76940;

extern SpuVoice76940 D_8009E0B8[];
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_short *D_8009A588;
extern u_char D_801E42F8;
extern SvmCurrent76940 D_801E4BD0;
extern u_short D_801F2A08;
extern u_short D_801F2A0C;

static inline u_short get_selected_voice(void) {
    return D_801E4BD0.voice;
}

long SpuVmSeKeyOff(short seq_sep, short vab_id, short program, u_short note) asm("func_80076940");
long SpuVmSeKeyOff(short seq_sep, short vab_id, short program, u_short note) {
    u_short bits_upper;
    u_short bits_lower;
    u_char voice;
    long count;
    u_short selected_voice;
    u_long selected_index;

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
                selected_index = selected_voice;
                if (selected_index < 0x10) {
                    bits_lower = 1 << selected_index;
                    bits_upper = 0;
                } else {
                    bits_lower = 0;
                    bits_upper = 1 << (selected_index - 0x10);
                }

                D_8009E0B8[selected_voice].active = 0;
                D_8009E0B8[selected_voice].pitch = 0;
                D_8009E0B8[selected_voice].vag = 0;

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
