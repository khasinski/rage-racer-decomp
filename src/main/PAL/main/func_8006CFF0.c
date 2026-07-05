#include "common.h"
#include "psyq/cd.h"

extern s32 D_801F1850;
extern s32 D_801E6C98;
extern StHEADER_RR *D_801E8AAC;

u32 StFreeRing(u32 *base) {
    s32 temp_a1;
    s32 i;
    s16 nSectors;
    StHEADER_RR *temp_v0;
    StHEADER_RR *temp_v0_2;

    temp_a1 = (base - (u32 *)&D_801E8AAC[D_801F1850]) / 504;
    temp_v0 = &D_801E8AAC[temp_a1];
    nSectors = D_801E8AAC[temp_a1].nSectors;
    if ((s16)temp_v0->id != 4) {
        return 1;
    }

    for (i = 0; i < nSectors;) {
        temp_v0_2 = &D_801E8AAC[i + temp_a1];
        i++;
        *(s16 *)temp_v0_2 = 0;
    }

    D_801E6C98 = i + temp_a1;
    return 0;
}
