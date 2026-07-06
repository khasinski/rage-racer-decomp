#include "psyq/spu.h"

extern u8 D_801E4D88;
extern s16 D_801E3FB0;
extern volatile u8 D_801E42F8;
extern u8 D_801E4CFC[];
extern u16 D_801F17AC;
extern u32 D_801F17BC[];

void SsSetReservedVoice(u8 arg0) asm("func_80072AD0");

void SsSetReservedVoice(u8 arg0) {
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

u8 SsSetVoiceCount(u8 voices) asm("func_80072B04");

u8 SsSetVoiceCount(u8 voices) {
    if (voices >= 25 || voices == 0) {
        return -1;
    }

    D_801E42F8 = voices;
    return D_801E42F8;
}

void SsVabClose(s16 vab_id) asm("func_80072B3C");

void SsVabClose(s16 vab_id) {
    s16 id;

    if ((u16)vab_id < 16) {
        id = vab_id;
        if (D_801E4CFC[id] == 1) {
            SpuFree(D_801F17BC[id]);
            D_801E4CFC[id] = 0;
            D_801F17AC--;
        }
    }
}
