#include "psyq/spu.h"

extern u_char D_801E4D88;
extern short D_801E3FB0;
extern volatile u_char D_801E42F8;
extern u_char D_801E4CFC[];
extern u_short D_801F17AC;
extern u_long D_801F17BC[];

void SsSetReservedVoice(u_char arg0) asm("func_80072AD0");

void SsSetReservedVoice(u_char arg0) {
    D_801E4D88 = arg0;
}

void SsSetMono(void) asm("func_80072AE0");

void SsSetMono(void) {
    D_801E3FB0 = 1;
}

void SsSetStereo(void) asm("func_80072AF4");

void SsSetStereo(void) {
    D_801E3FB0 = 0;
}

u_char SsSetVoiceCount(u_char voices) asm("func_80072B04");

u_char SsSetVoiceCount(u_char voices) {
    if (voices >= 25 || voices == 0) {
        return -1;
    }

    D_801E42F8 = voices;
    return D_801E42F8;
}

void SsVabClose(short vab_id) asm("func_80072B3C");

void SsVabClose(short vab_id) {
    short id;

    if ((u_short)vab_id < 16) {
        id = vab_id;
        if (D_801E4CFC[id] == 1) {
            SpuFree(D_801F17BC[id]);
            D_801E4CFC[id] = 0;
            D_801F17AC--;
        }
    }
}
